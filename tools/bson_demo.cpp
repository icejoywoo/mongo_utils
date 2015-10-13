#include <iostream>
#include "mongo/bson/bson.h"
#include "mongo/client/dbclient.h" // for the driver

int main(int argc, char* argv[]) {
    mongo::BSONObjBuilder b;
    b.append("name", "Joe");
    b << "age" << 33;
    mongo::BSONObj p = b.obj();
    std::cout << p << std::endl;

    // oid: https://jira.mongodb.org/browse/CXX-561
    // You must initialize the driver to ensure that Mongo initializer blocks are executed.
    // You can do this by calling mongo::client::initialize or stack allocating a
    // mongo::client::GlobalInstance in the main() block of your program.
    mongo::client::initialize();
    //mongo::client::GlobalInstance instance;
    // Use the GENOID helper to add an object id to your object.
    mongo::BSONObj p2 = BSON(mongo::GENOID << "name" << "John" << "age" << 24);
    std::cout << p2 << std::endl;

    mongo::BSONObj p3 = mongo::BSONObjBuilder().genOID().append("name", "Tom").append("age", 29).obj();
    std::cout << p3 << std::endl;
}
