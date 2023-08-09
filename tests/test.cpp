#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "include/doctest.h"
#include <iostream>
#include "../src/utils.cpp"


TEST_SUITE_BEGIN("Utils");

TEST_CASE("String Utils"){
    SUBCASE("Trim Test"){
        CHECK(utils::string::trim("  hello  ") == "hello");
        CHECK(utils::string::trim("  hello") == "hello");
        CHECK(utils::string::trim("hello  ") == "hello"); 
        CHECK(utils::string::trim("hello") == "hello");
        CHECK(utils::string::trim("  hello world  ") == "hello world");
        CHECK(utils::string::trim("  hello world") == "hello world");
        CHECK(utils::string::trim("hello world  ") == "hello world");
        CHECK(utils::string::trim("hello world") == "hello world");
    }
    SUBCASE("Split String"){
        std::string str = "hello world";
        auto strArr = utils::string::splitString(str, " ");
        CHECK(strArr[0] == "hello");

        str = "id:num,name:str";
        strArr = utils::string::splitString(str, ",");
        CHECK(strArr[0] == "id:num");
        CHECK(strArr[1] == "name:str");
    }
}

TEST_SUITE_END();