#include <iostream>
#include <utility>

#include "rxcpp/rx.hpp"
#include "src/crow_all.h"

#include "src/model.hpp"
#include "src/dao.hpp"
#include "src/http.hpp"

using namespace std;
using namespace market::model;
using namespace market::dao;
using namespace market::http;
namespace rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}
using namespace rx;

/*
 * Http requests
 * add user     : localhost:8080/add_user?name=Joe&currency=eur
 * add product  : localhost:8080/add_product?name=cam&price=1000
 * show catalog : localhost:8080/catalog?name=Joe
 */

int main() {
    // Mongo config
    mongocxx::instance instance{};
    // Connect to MongoDB server
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    // Connect to database
    auto db = client["reactive-market"];
    // Get collections
    auto users_db      = db["users"];
    auto products_db   = db["products"];
    reactive_mongo dao(users_db, products_db);
    // Local web-application
    crow::SimpleApp app;
    // Market service
    add_route(app, dao);
    // Run
    app.port(8080)
            .server_name("market")
            .multithreaded()
            .run();
    return 0;
}
