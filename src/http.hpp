//
// Created by Alex Shchelochkov on 31.03.2023.
//

#pragma once

#include "crow_all.h"

#include "model.hpp"
#include "dao.hpp"

namespace market::http {
    static void add_route(crow::SimpleApp& app, dao::reactive_mongo& dao) {
        CROW_ROUTE(app, "/<string>")
                ([&](const crow::request& req, const std::string& str) {
                    if (str == "add_user") {
                        std::string name;
                        model::currency_t cur;
                        if (req.url_params.get("name") != nullptr)
                            name = req.url_params.get("name");
                        else return crow::response(400);
                        if (req.url_params.get("currency") != nullptr)
                            cur = model::parse(req.url_params.get("currency"));
                        else return crow::response(400);
                        auto res = dao.insert_user(model::user_t(name, cur));
                        std::string response;
                        res.subscribe(
                                [&](const mongocxx::stdx::optional<mongocxx::result::insert_one>& v){
                                    if (v) response = "Success";
                                    else response = "Error while inserting user";
                                });
                        return crow::response(response);
                    } else if (str == "add_product") {
                        std::string name;
                        double price;
                        if (req.url_params.get("name") != nullptr)
                            name = req.url_params.get("name");
                        else return crow::response(400);
                        if (req.url_params.get("price") != nullptr)
                            price = crow::lexical_cast<double>(req.url_params.get("price"));
                        else return crow::response(400);
                        auto res = dao.insert_product(model::product_t(name, price));
                        std::string response;
                        res.subscribe(
                                [&](const mongocxx::stdx::optional<mongocxx::result::insert_one>& v){
                                    if (v) response = "Success";
                                    else response = "Error while inserting product";
                                });
                        return crow::response(response);
                    } else if (str == "catalog") {
                        std::string name;
                        if (req.url_params.get("name") != nullptr)
                            name = req.url_params.get("name");
                        else return crow::response(400);
                        auto user_tmp = dao.get_user(name);
                        model::user_t user("", model::currency_t::RUB);
                        user_tmp.subscribe([&](model::user_t u){ user = std::move(u); });
                        auto products = dao.get_products();
                        rxcpp::observable<std::string> products_str = products.map(
                                [&](const model::product_t& p){
                                    return p.get_name() + " : "
                                           + std::to_string(convert(p.get_price_in_rub(), user.get_currency()))
                                           + " " + to_string(user.get_currency()); });
                        auto header = rxcpp::observable<>::from<>("User " + user.get_name() + " : ");
                        rxcpp::observable<std::string> response = header.concat(products_str.as_dynamic());
                        std::string res;
                        response.subscribe(
                                [&](std::string s){
                                    res += (res.empty() ? "" : "\n") + std::move(s);
                                });
                        return crow::response(res);
                    } else return crow::response(404);
                });
    }
}
