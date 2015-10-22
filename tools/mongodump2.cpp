/**
 * @author: wujiabin
 * @brief: dump shard集群(不支持replica set)的数据, 从mongos中dump的速度非常慢, 针对mongo 2.6
 */

#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "blocking_queue.h"
#include "boost/atomic.hpp"
#include "boost/bind.hpp"
#include "boost/program_options.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include "mongo/bson/bson.h"
#include "mongo/bson/bsonelement.h"
#include "mongo/client/dbclient.h" // for the driver


bool isMongos(mongo::DBClientConnection& mongo) {
    mongo::BSONObj ret;
    if (mongo.runCommand("admin", BSON("serverStatus" << 1), ret)) {
        // std::cout << "serverStatus: " << ret << std::endl;
        return ret.getStringField("process") == std::string("mongos");
    } else {
        std::cerr << "Failed to run serverStatus: " << ret << std::endl;
        throw "Failed to run serverStatus";
    }
}

base::BoundedBlockingQueue<boost::shared_ptr<mongo::BSONObj> > q(10000);

void dump_mongod(const std::string& mongo_uri, const std::string& ns) {
    mongo::DBClientConnection mongo;
    mongo.connect(mongo_uri);

    std::cout << "dump " << mongo_uri << std::endl;
    std::auto_ptr<mongo::DBClientCursor> cursor = mongo.query(ns, mongo::BSONObj());
    while (cursor->more()) {
        mongo::BSONObj d = cursor->next();
        // 单独申请一块buffer存bsonobj的数据
        char* buffer = new char[d.objsize()];
        memcpy(buffer, d.objdata(), d.objsize());
        q.put(boost::shared_ptr<mongo::BSONObj>(new mongo::BSONObj(buffer)));
    }
}

void dump_writer(int total, const std::string& output) {
    std::fstream out(output.c_str(), std::fstream::out | std::fstream::binary);
    std::cout << "dump writer " << std::endl;
    int counter = 0;
    int invalid_counter = 0;
    while (counter < total) {
        boost::shared_ptr<mongo::BSONObj> d = q.take();
        if (d->isValid()) {
            out.write(d->objdata(), d->objsize());
        } else {
            ++invalid_counter;
        }
        // 删除之前申请的存放bsonobj数据的内存
        delete d->objdata();
        ++counter;
    }
    std::cout << "counter: " << counter << ", invalid counter: " << invalid_counter << std::endl;
    out.close();
}

int main(int argc, char* argv[]) {
    namespace po = boost::program_options;
    po::options_description desc("Usage: dump mongodb data.\nAllowed options");
    desc.add_options()
            ("help", "produce help message")
            ("host,h", po::value<std::string>()->default_value("localhost"), "mongodb host to connect to (setname/host1,host2 for replica sets)")
            ("port,p", po::value<int>()->default_value(27017), "server port (can also use --host hostname:port)")
            ("db,d", po::value<std::string>(), "database to use")
            ("collection,c", po::value<std::string>(), "collection to use")
            ("out,o", po::value<std::string>()->default_value("dump.bson"), "output file (defaults to 'dump.bson')")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    mongo::client::initialize();

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    std::string host = vm["host"].as<std::string>();
    int port = vm["port"].as<int>();
    std::string db;
    if (vm.count("db")) {
        db = vm["db"].as<std::string>();
    } else {
        std::cerr << "db must be set." << std::endl;
        return 1;
    }
    std::string collection;
    if (vm.count("collection")) {
        collection = vm["collection"].as<std::string>();
    } else {
        std::cerr << "collection must be set." << std::endl;
        return 1;
    }
    std::string out = vm["out"].as<std::string>();

    std::string mongodb;
    if (host.find(':') != -1) {
        mongodb = host;
    } else {
        std::stringstream ss;
        ss << host << ':' << port;
        mongodb = ss.str();
    }

    std::cout << "mongodb: " << mongodb << std::endl
              << "db: " << db << std::endl
              << "collection: " << collection << std::endl;

    try {
        mongo::DBClientConnection c;
        c.connect(mongodb);
        std::cout << "mongodb connected ok" << std::endl;

        bool is_mongos = isMongos(c);
        std::stringstream ss;
        ss << db << '.' << collection;
        std::string ns = ss.str();
        int total_count = c.count(ns);

        if (is_mongos) {
            std::cout << "mongos" << std::endl;
            // get shards
            std::map<std::string, std::string> shards;
            std::auto_ptr<mongo::DBClientCursor> cursor = c.query("config.shards", mongo::BSONObj());
            while (cursor->more()) {
                mongo::BSONObj d = cursor->next();
                std::cout << "Dealing: " << d.jsonString() << std::endl;
                std::string shard_name = d.getStringField("_id");
                // 对于没有replica set的shard来说就是shard mongodb uri
                std::string shard_mongo = d.getStringField("host");
                shards[shard_name] = shard_mongo;
            }

            boost::thread_group threads;
            for (std::map<std::string, std::string>::iterator it = shards.begin();
                 it != shards.end(); ++it) {
                std::string shard_name = it->first;
                std::string shard_uri = it->second;

                std::cout << "dump " << shard_uri << std::endl;
                threads.add_thread(new boost::thread(dump_mongod, shard_uri, ns));
            }

            boost::thread t(dump_writer, total_count, out);
            threads.join_all();
            t.join();
        } else {
            std::cout << "mongod" << std::endl;
            boost::thread t(dump_mongod, mongodb, ns);
            boost::thread w(dump_writer, total_count, out);
            t.join();
            w.join();
        }

        std::cout << "config.shards count:" << c.count("config.shards") << std::endl;
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}

