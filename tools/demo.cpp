#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h" // for the driver
#include "glog/logging.h"
#include "gflags/gflags.h"

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");

void run() {
  mongo::DBClientConnection c;
  c.connect("localhost");
}

int main(int argc, char* argv[]) {
    FLAGS_log_dir = "./log";
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("demo");

    std::cout << FLAGS_big_menu << std::endl;

    mongo::client::initialize();
    try {
        run();
        std::cout << "connected ok" << std::endl;
        LOG(INFO) << "connected ok";
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
        LOG(WARNING) << "caught " << e.what();
    }
    google::ShutdownGoogleLogging();
    return EXIT_SUCCESS;
}
