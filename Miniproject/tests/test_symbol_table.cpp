#include "symbol_table.hpp"

#include <doctest/doctest.h>

#include <stdexcept>
#include <string>

using stochastic::SymbolTable;

TEST_CASE("SymbolTable: insert and lookup")
{
    SymbolTable<std::string, int> t;
    t.insert("a", 1);
    t.insert("b", 2);

    CHECK(t.size() == 2);
    CHECK(t.contains("a"));
    CHECK(t.lookup("a") == 1);
    CHECK(t.lookup("b") == 2);
}

TEST_CASE("SymbolTable: empty()")
{
    SymbolTable<std::string, int> t;
    CHECK(t.empty());
    t.insert("x", 42);
    CHECK_FALSE(t.empty());
}

TEST_CASE("SymbolTable: duplicate insert throws std::invalid_argument")
{
    SymbolTable<std::string, int> t;
    t.insert("a", 1);
    CHECK_THROWS_AS(t.insert("a", 99), std::invalid_argument);
    // The original value must remain.
    CHECK(t.lookup("a") == 1);
}

TEST_CASE("SymbolTable: missing lookup throws std::out_of_range")
{
    SymbolTable<std::string, int> t;
    CHECK_THROWS_AS(static_cast<void>(t.lookup("missing")), std::out_of_range);

    t.insert("a", 1);
    CHECK_THROWS_AS(static_cast<void>(t.lookup("b")), std::out_of_range);
}

TEST_CASE("SymbolTable: works with non-string key types (int)")
{
    SymbolTable<int, std::string> t;
    t.insert(1, "one");
    t.insert(2, "two");
    CHECK(t.lookup(1) == "one");
    CHECK_THROWS_AS(static_cast<void>(t.lookup(3)), std::out_of_range);
}
