//
// Created by Alex Shchelochkov on 30.03.2023.
//

#pragma once

#include <string>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>
#include <ostream>
#include <utility>

namespace market::model {
    enum class currency_t { RUB, USD, EUR };

     static std::string to_string(currency_t currency) {
        switch (currency) {
            case currency_t::RUB    : return "rub";
            case currency_t::USD    : return "usd";
            case currency_t::EUR    : return "eur";
            default                 : return "rub";
        }
    }

    static currency_t parse(const std::string& str) {
        if (str == "eur") return currency_t::EUR;
        if (str == "usd") return currency_t::USD;
        if (str == "rub") return currency_t::RUB;
        throw std::invalid_argument("Unknown currency : " + str);
    }

    class user_t {
    private:
        std::string name;
        currency_t currency;
    public:
        user_t(std::string name, currency_t currency)
        : name(std::move(name)), currency(currency)
        {}
        explicit user_t(bsoncxx::document::view doc)
        : user_t(doc["name"].get_string().value.to_string(), parse(doc["currency"].get_string().value.to_string()))
        {}
        std::string get_name() const {
            return name;
        }
        currency_t get_currency() const {
            return currency;
        }
        bsoncxx::document::value to_document() const {
            return bsoncxx::builder::basic::make_document(
                    bsoncxx::builder::basic::kvp("name", name),
                    bsoncxx::builder::basic::kvp("currency", model::to_string(currency)));
        }
        std::string to_string() {
            return "User { name='" + name + "', currency=" + model::to_string(currency) +" }";
        }
    };

    class product_t {
    private:
        std::string name;
        double price_in_rub;
    public:
        product_t(const std::string &name, double priceInRub)
        : name(name), price_in_rub(priceInRub)
        {}
        explicit product_t(bsoncxx::document::view doc)
        : product_t(doc["name"].get_string().value.to_string(), doc["price"].get_double())
        {}
        std::string get_name() const {
            return name;
        }
        double get_price_in_rub() const {
            return price_in_rub;
        }
        bsoncxx::document::value to_document() const {
            return bsoncxx::builder::basic::make_document(
                    bsoncxx::builder::basic::kvp("name", name),
                    bsoncxx::builder::basic::kvp("price", price_in_rub));
        }
        std::string to_string() {
            return "Product { name='" + name + "', price=" + std::to_string(price_in_rub) +" }";
        }
    };

    static double convert(double val, currency_t to) {
        switch (to) {
            case currency_t::EUR    : return val * 0.012;
            case currency_t::USD    : return val * 0.013;
            default                 : return val;
        }
    }
}
