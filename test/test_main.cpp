//
// Created by Alex Shchelochkov on 26.03.2023.
//

#include <gtest/gtest.h>
#include <mongocxx/instance.hpp>

GTEST_API_ int main(int argc, char ** argv) {
    mongocxx::instance instance{};
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}