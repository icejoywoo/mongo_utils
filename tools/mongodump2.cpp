/**
 * @author: wujiabin
 * @brief: dump shard集群(不支持replica set)的数据, 从mongos中dump的速度非常慢, 针对mongo 2.6
 */

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "blocking_queue.h"
#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include "gflags/gflags.h"
#include "mongo/bson/bson.h"
#include "mongo/client/dbclient.h" // for the driver

DEFINE_string(mongodb, "localhost:27017", "mongodb uri");
DEFINE_string(db, "config", "db");
DEFINE_string(collection, "shards", "collection");
DEFINE_string(output, "output.dump", "output file");

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

base::BoundedBlockingQueue<mongo::BSONObj> q(10000);

void dump_mongod(const std::string& mongo_uri) {
    mongo::DBClientConnection mongo;
    mongo.connect(mongo_uri);

    std::stringstream ss;
    ss << FLAGS_db << '.' << FLAGS_collection;
    std::string ns = ss.str();

    std::auto_ptr<mongo::DBClientCursor> cursor = mongo.query(ns, mongo::BSONObj());
    while (cursor->more()) {
        mongo::BSONObj d = cursor->next();
        q.put(d);
    }
}

void dump_writer() {
    std::fstream out(FLAGS_output.c_str(), std::fstream::out | std::fstream::binary);
    while (true) {
        mongo::BSONObj d = q.take();
        out.write(d.objdata(), d.objsize());
    }
    out.close();
}

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    mongo::client::initialize();

    std::cout << "mongodb: " << FLAGS_mongodb << std::endl
              << "db: " << FLAGS_db << std::endl
              << "collection: " << FLAGS_collection << std::endl;

    try {
        mongo::DBClientConnection c;
        c.connect(FLAGS_mongodb);
        std::cout << "mongodb connected ok" << std::endl;

        bool is_mongos = isMongos(c);

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
            std::vector<boost::thread> threads;
            for (std::map<std::string, std::string>::iterator it = shards.begin();
                 it != shards.end(); ++it) {
                std::string shard_name = it->first;
                std::string shard_uri = it->second;

                std::cout << "dump " << shard_uri << std::endl;
                boost::thread t(dump_mongod, shard_uri);
                threads.push_back(t);
            }

            boost::thread t(dump_writer);

            for (std::vector<boost::thread>::iterator it = threads.begin();
                 it != threads.end(); ++it) {
                it->join();
            }
        } else {
            std::cout << "mongod" << std::endl;
            boost::thread t(dump_mongod, FLAGS_mongodb);
            boost::thread w(dump_writer);
            t.join();
        }

        std::cout << "config.shards count:" << c.count("config.shards") << std::endl;
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}

