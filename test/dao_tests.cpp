//
// Created by Alex Shchelochkov on 26.03.2023.
//

#include <gtest/gtest.h>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include "../src/model.hpp"
#include "../src/dao.hpp"

using namespace std;
using namespace market;
using namespace market::model;
using namespace market::dao;

TEST(dao_test, add_user) {
    // Connect to MongoDB server
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    // Connect to database
    auto db = client["test-reactive-market"];
    auto dao = reactive_mongo(db["users"], db["products"]);

    auto obs = dao.insert_user(user_t("Alex", currency_t::USD));
    obs.subscribe(
            [](mongocxx::stdx::optional<mongocxx::result::insert_one> o){
                EXPECT_TRUE(o);
            });

    auto u = dao.get_user("Alex");
    u.subscribe(
            [](user_t u){
                EXPECT_EQ("Alex", u.get_name());
                EXPECT_EQ(currency_t::USD, u.get_currency());
            });

    db["users"].drop();
    db["products"].drop();
}

TEST(dao_test, add_product) {
    // Connect to MongoDB server
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    // Connect to database
    auto db = client["test-reactive-market"];
    auto dao = reactive_mongo(db["users"], db["products"]);

    auto obs = dao.insert_product(product_t("cam", 100));
    obs.subscribe(
            [](mongocxx::stdx::optional<mongocxx::result::insert_one> o){
                EXPECT_TRUE(o);
            });

    auto p = dao.get_products();
    p.subscribe(
            [](product_t p){
                EXPECT_EQ("cam", p.get_name());
                EXPECT_EQ(100, p.get_price_in_rub());
            });

    db["users"].drop();
    db["products"].drop();
}