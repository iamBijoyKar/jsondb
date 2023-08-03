#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "include/doctest.h"
#include <iostream>


TEST_CASE("main") { std::cout << "hello from <main.cpp>" << std::endl; }