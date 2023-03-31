//
// Created by Alex Shchelochkov on 27.03.2023.
//

#include <gtest/gtest.h>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include "../src/crow_all.h"

#include "../src/model.hpp"
#include "../src/dao.hpp"
#include "../src/http.hpp"

using namespace std;

TEST(market_test, all_functions) {
    // Connect to MongoDB server
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    // Connect to database
    auto db = client["test-reactive-market"];
    auto dao = market::dao::reactive_mongo(db["users"], db["products"]);

    // Local web-application
    crow::SimpleApp app;
    // Report service
    market::http::add_route(app, dao);

    app.validate();
    {
        crow::request req;
        crow::response res;
        // Correct add_user
        req.url = "/add_user";
        req.url_params = crow::query_string("?name=Joe&currency=eur");
        app.handle(req, res);
        EXPECT_EQ(res.code, 200);
        EXPECT_EQ(res.body, "Success");
        // Correct add_product
        req.url = "/add_product";
        req.url_params = crow::query_string("?name=cam&price=1000");
        app.handle(req, res);
        EXPECT_EQ(res.code, 200);
        EXPECT_EQ(res.body, "Success");
        // Correct catalog
        req.url = "/catalog";
        req.url_params = crow::query_string("?name=Joe");
        app.handle(req, res);
        EXPECT_EQ(res.code, 200);
        EXPECT_EQ(res.body, "User Joe : \ncam : 12.000000 eur");
        // Invalid argument add_user
        req.url = "/add_user";
        req.url_params = crow::query_string("?id=1");
        app.handle(req, res);
        EXPECT_EQ(res.code, 400);
        // Invalid argument add_product
        req.url = "/add_product";
        req.url_params = crow::query_string("?id=1");
        app.handle(req, res);
        EXPECT_EQ(res.code, 400);
        // Invalid argument catalog
        req.url = "/catalog";
        req.url_params = crow::query_string("?id=1");
        app.handle(req, res);
        EXPECT_EQ(res.code, 400);
        // Invalid request
        req.url = "/cat";
        app.handle(req, res);
        EXPECT_EQ(res.code, 404);
    }
}