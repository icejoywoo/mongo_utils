#include <cstdlib>
#include <iostream>

#include "boost/smart_ptr.hpp"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "mongo/bson/bson.h"
#include "mongo/client/dbclient.h" // for the driver

// cmdline: --big_menu makes big_menu is true, and --nobig_menu false.
DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");

int main(int argc, char* argv[]) {
    FLAGS_log_dir = "./log";
    gflags::SetVersionString("0.0.0.1 alpha");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("demo");

    std::cout << FLAGS_big_menu << std::endl;

    mongo::client::initialize();
    try {
        mongo::DBClientConnection c;
        c.connect("localhost");
        std::cout << "connected ok" << std::endl;
        LOG(INFO) << "connected ok";
        // insert a doc
        mongo::BSONObj p = BSON(mongo::GENOID << "name" << "John" << "age" << 24);
        c.insert("test.persons", p);

        // query
        std::auto_ptr<mongo::DBClientCursor> cursor = c.query("test.persons", mongo::BSONObj());
        while (cursor->more()) {
            std::cout << cursor->next().toString() << std::endl;
        }
        std::cout << "test.persons count:" << c.count("test.persons") << std::endl;
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
        LOG(WARNING) << "caught " << e.what();
    }
    google::ShutdownGoogleLogging();
    return EXIT_SUCCESS;
}
