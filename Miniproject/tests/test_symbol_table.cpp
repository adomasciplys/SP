#include "symbol_table.hpp"

#include <doctest/doctest.h>

#include <stdexcept>
#include <string>

using stochastic::Hashable;
using stochastic::SymbolTable;
using stochastic::SymbolTableKey;

// compile-time tests of the concept itself
// The NoEq and NoHash checks were developed using AI
namespace {

// Hash works, equality does not (no operator==), so SymbolTableKey rejects it.
struct NoEq { int x; };

// Equality works, hash does not (no std::hash<NoHash> specialization).
struct NoHash {
    int x;
    bool operator==(const NoHash&) const = default;
};
}  // namespace

// Specialize std::hash for NoEq
template <>
struct std::hash<NoEq>
{
    std::size_t operator()(const NoEq& v) const noexcept { return std::hash<int>{}(v.x); }
};

// Positive: every common type satisfies both concepts.
static_assert(Hashable<int>);
static_assert(Hashable<char>);
static_assert(Hashable<bool>);
static_assert(Hashable<double>);
static_assert(Hashable<unsigned long>);
static_assert(Hashable<void*>);
static_assert(Hashable<std::string>);

static_assert(SymbolTableKey<int>);
static_assert(SymbolTableKey<char>);
static_assert(SymbolTableKey<bool>);
static_assert(SymbolTableKey<double>);
static_assert(SymbolTableKey<unsigned long>);
static_assert(SymbolTableKey<void*>);
static_assert(SymbolTableKey<std::string>);

// Negative: exactly one half is missing in each case.
static_assert(!Hashable<NoHash>);
static_assert(!SymbolTableKey<NoHash>);  // fails on the hash side
static_assert(!SymbolTableKey<NoEq>);    // fails on the equality side

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

TEST_CASE("SymbolTable: primitive Key and Value (int -> double)")
{
    SymbolTable<int, double> t;
    t.insert(1, 1.5);
    t.insert(2, 2.5);

    CHECK(t.size() == 2);
    CHECK(t.lookup(1) == doctest::Approx(1.5));
    CHECK(t.lookup(2) == doctest::Approx(2.5));
    CHECK_THROWS_AS(t.insert(1, 9.9), std::invalid_argument);                 // duplicate
    CHECK_THROWS_AS(static_cast<void>(t.lookup(99)), std::out_of_range);      // missing
}
