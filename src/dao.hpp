//
// Created by Alex Shchelochkov on 30.03.2023.
//

#pragma once

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include "rxcpp/rx.hpp"

#include "model.hpp"

namespace market::dao {
    static const std::string DATABASE_NAME = "reactive-market";
    static const std::string USERS_COL = "users";
    static const std::string PRODUCTS_COL = "products";

    template<class T>
    static std::vector<T> to_vector(mongocxx::cursor c) {
        std::vector<T> res;
        for (auto& v : c)
            res.template emplace_back(v);
        return res;
    }

    class reactive_mongo {
    private:
        mongocxx::collection users;
        mongocxx::collection products;
    public:
        reactive_mongo(mongocxx::collection users, mongocxx::collection products)
        : users(std::move(users)), products(std::move(products))
        {}
        rxcpp::observable<model::user_t> get_user(std::string name) {
            return rxcpp::observable<>::from<>(
                    model::user_t(
                            users.find_one(
                                    bsoncxx::builder::basic::make_document(
                                            bsoncxx::builder::basic::kvp("name",  name)
                                    )
                            )->view()
                    )
            );
        }
        rxcpp::observable<model::product_t> get_products() {
            return rxcpp::observable<>::iterate(to_vector<model::product_t>(products.find({})));
        }
        rxcpp::observable<mongocxx::stdx::optional<mongocxx::result::insert_one>> insert_user(model::user_t user) {
            return rxcpp::observable<>::from<>(users.insert_one(user.to_document()));
        }
        rxcpp::observable<mongocxx::stdx::optional<mongocxx::result::insert_one>> insert_product(model::product_t product) {
            return rxcpp::observable<>::from<>(products.insert_one(product.to_document()));
        }
    };
}
