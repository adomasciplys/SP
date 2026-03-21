#include "json_input.hpp"
#include "meta.hpp"
#include "data.hpp"

#include <map>
#include <doctest/doctest.h>

#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std::string_literals;

// NOLINTBEGIN

TEST_CASE("JSON input")
{
    SUBCASE("boolean: true")
    {
        auto is = std::istringstream{"true"};
        auto v = false;
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == true);
    }
    SUBCASE("boolean: false")
    {
        auto is = std::istringstream{"false"};
        auto v = true;
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == false);
    }
    SUBCASE("integer")
    {
        auto is = std::istringstream{"7"};
        auto v = 0;
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == 7);
    }
    SUBCASE("double")
    {
        auto is = std::istringstream{"3.14"};
        auto v = 0.0;
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == 3.14);
    }
    SUBCASE("cpp-string")
    {
        auto is = std::istringstream{"\"hello\""};
        auto v = std::string{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == "hello");
    }
    SUBCASE("container")
    {
        auto is = std::istringstream{"[3,7,11]"};
        auto v = std::vector<int>{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == std::vector{3, 7, 11});
    }
    SUBCASE("map: simple")
    {
        auto is = std::istringstream{R"({"one":1,"two":2})"};
        auto v = std::map<std::string, int>{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == std::map<std::string, int>{{"one", 1}, {"two", 2}});
    }
    SUBCASE("map: empty")
    {
        auto is = std::istringstream{"{}"};
        auto v = std::map<std::string, int>{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v.empty());
    }
    SUBCASE("map: complex values")
    {
        auto is = std::istringstream{R"({"a":[1,2,3],"b":[4,5,6]})"};
        auto v = std::map<std::string, std::vector<int>>{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == std::map<std::string, std::vector<int>>{{"a", {1, 2, 3}}, {"b", {4, 5, 6}}});
    }
    SUBCASE("unordered_map")
    {
        auto is = std::istringstream{R"({"x":10,"y":20})"};
        auto v = std::unordered_map<std::string, int>{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v.size() == 2);
        CHECK(v["x"] == 10);
        CHECK(v["y"] == 20);
    }


    static_assert(accepts_v<Aggregate&, json_reader>, "Aggregate should accept reader");
    static_assert(accepts_v<Nested&, json_reader>, "Nested should accept reader");
    // TODO: uncomment the following extra tests for meta library and fix accepts_v implementation
    // static_assert(!accepts_v<int, double>, "int should not accept double");
    SUBCASE("Aggregate")
    {
        auto is = std::istringstream{R"({"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]})"};
        auto v = Aggregate{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == Aggregate{true, 3, 3.14, "hello", {7, 11}});
    }
    SUBCASE("Nested")
    {
        auto is =
            std::istringstream{R"({"text":"complicated","agg":{"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]}})"};
        auto v = Nested{};
        json_istream{is} >> v;
        CHECK(is);
        CHECK(v == Nested{"complicated", {true, 3, 3.14, "hello", {7, 11}}});
    }
}

// NOLINTEND