/// Created by Marius Mikucionis <marius@cs.aau.dk>

/** Unit tests for NumberSeries */

#include "number_series.hpp"

#include <doctest/doctest.h>

#include <sstream>

using namespace DataSeries;

TEST_SUITE_BEGIN("Number Series Test");

/// NumberSeries class
TEST_CASE("Maintain minimum and maximum values")
{
    auto ns = NumberSeries{};
    ns.add_value(10);
    CHECK(ns.min() == 10);
    CHECK(ns.max() == 10);

    SUBCASE("Add greater")
    {
        ns.add_value(15);
        CHECK(ns.min() == 10);
        CHECK(ns.max() == 15);
        ns.add_value(17);
        CHECK(ns.min() == 10);
        CHECK(ns.max() == 17);
        ns.add_value(13);
        CHECK(ns.min() == 10);
        CHECK(ns.max() == 17);
    }
    SUBCASE("Add lesser")
    {
        ns.add_value(5);
        CHECK(ns.min() == 5);
        CHECK(ns.max() == 10);
        ns.add_value(3);
        CHECK(ns.min() == 3);
        CHECK(ns.max() == 10);
        ns.add_value(7);
        CHECK(ns.min() == 3);
        CHECK(ns.max() == 10);
    }
}

TEST_CASE("Special members: ctors, dtor, assignment")
{
    const auto ns1 = NumberSeries{11, 3, 7};
    REQUIRE(ns1.size() == 3);
    CHECK(ns1.min() == 3);
    CHECK(ns1.max() == 11);
    auto ns2 = NumberSeries{27, 20, 33, 23};
    REQUIRE(ns2.size() == 4);
    CHECK(ns2.min() == 20);
    CHECK(ns2.max() == 33);
    auto ns3 = ns1;
    REQUIRE(ns3.size() == 3);
    CHECK(ns3.min() == 3);
    CHECK(ns3.max() == 11);
    ns2 = std::move(ns3);
    REQUIRE(ns2.size() == 3);
    CHECK(ns2.min() == 3);
    CHECK(ns2.max() == 11);
    CHECK(ns3.size() == 0);  // your implementation may differ or even crash!
}

TEST_CASE("Static factory method")
{
    const auto ns = NumberSeries::make_random(4);
    REQUIRE(ns.size() == 4);
}


TEST_CASE("operator+ and operator+= over number series")
{
    const auto ns1 = NumberSeries{1, 2};
    REQUIRE(ns1.size() == 2);
    REQUIRE(ns1.amplitude() == 1);
    const auto ns2 = NumberSeries{3, 4, 5};
    REQUIRE(ns2.size() == 3);
    REQUIRE(ns2.amplitude() == 2);
    auto ns3 = ns1 + ns2;  // [1,2] + [3,4,5] = [4,6,5]
    REQUIRE(ns3.size() == 3);
    REQUIRE(ns3.amplitude() == 2);
    ns3.add_value(10);  // [4,6,5,10]
    REQUIRE(ns3.size() == 4);
    REQUIRE(ns3.amplitude() == 6);
    (ns3 += ns1) += ns2;  // [4,6,5,10] + [1,2] + [3,4,5] = [8,12,10,10]
    REQUIRE(ns3.size() == 4);
    CHECK(ns3.amplitude() == 4);
}



TEST_CASE("operator< using amplitudes")
{
    const auto ns1 = NumberSeries{6, 3, 9};
    CHECK(ns1.amplitude() == 6);
    const auto ns2 = NumberSeries{24, 21, 22};
    REQUIRE(ns2.amplitude() == 3);
    CHECK(ns2 < ns1);
}


TEST_CASE("Maintain minimum and maximum values")
{
    auto ns = NumberSeriesWrap{};
    ns.add_value(10);
    CHECK(ns.min() == 10);
    CHECK(ns.max() == 10);
    SUBCASE("Add greater")
    {
        ns.add_value(15);
        CHECK(ns.min() == 10);
        CHECK(ns.max() == 15);
        ns.add_value(17);
        CHECK(ns.min() == 10);
        CHECK(ns.max() == 17);
        ns.add_value(13);
        CHECK(ns.min() == 10);
        CHECK(ns.max() == 17);
    }
    SUBCASE("Add lesser")
    {
        ns.add_value(5);
        CHECK(ns.min() == 5);
        CHECK(ns.max() == 10);
        ns.add_value(3);
        CHECK(ns.min() == 3);
        CHECK(ns.max() == 10);
        ns.add_value(7);
        CHECK(ns.min() == 3);
        CHECK(ns.max() == 10);
    }
}


TEST_CASE("Special members: ctors, dtor, assignment")
{
    const auto ns1 = NumberSeriesWrap{11, 3, 7};
    CHECK(ns1.size() == 3);
    CHECK(ns1.min() == 3);
    CHECK(ns1.max() == 11);
    auto ns2 = NumberSeriesWrap{27, 20, 33, 23};
    CHECK(ns2.size() == 4);
    CHECK(ns2.min() == 20);
    CHECK(ns2.max() == 33);
    auto ns3 = ns1;
    CHECK(ns3.size() == 3);
    CHECK(ns3.min() == 3);
    CHECK(ns3.max() == 11);
    ns2 = std::move(ns3);
    CHECK(ns2.size() == 3);
    CHECK(ns2.min() == 3);
    CHECK(ns2.max() == 11);
    CHECK(ns3.size() == 4);  // your implementation may differ or even crash!
}


TEST_CASE("NumberSeriesWrap should also have a static factory method")
{
    const auto nsw = NumberSeriesWrap::make_random(4);
    REQUIRE(nsw.size() == 4);
}


TEST_CASE("operator+ and operator+= over number series")
{
    const auto ns1 = NumberSeriesWrap{1, 2};
    REQUIRE(ns1.size() == 2);
    REQUIRE(ns1.amplitude() == 1);
    const auto ns2 = NumberSeriesWrap{3, 4, 5};
    REQUIRE(ns2.size() == 3);
    REQUIRE(ns2.amplitude() == 2);
    auto ns3 = ns1 + ns2;  // [1,2] + [3,4,5] = [4,6,5]
    REQUIRE(ns3.size() == 3);
    REQUIRE(ns3.amplitude() == 2);
    ns3.add_value(10);  // [4,6,5,10]
    REQUIRE(ns3.size() == 4);
    REQUIRE(ns3.amplitude() == 6);
    (ns3 += ns1) += ns2;  // [4,6,5,10] + [1,2] + [3,4,5] = [8,12,10,10]
    REQUIRE(ns3.size() == 4);
    CHECK(ns3.amplitude() == 4);
}


TEST_CASE("operator< using amplitudes")
{
    const auto ns1 = NumberSeriesWrap{6, 3, 9};
    REQUIRE(ns1.amplitude() == 6);
    const auto ns2 = NumberSeriesWrap{24, 21, 22};
    REQUIRE(ns2.amplitude() == 3);
    CHECK(ns2 < ns1);
}

TEST_CASE("Memory Leak")
{
    auto os = std::ostringstream{};
    const auto* ns = new unsigned{0xDEADBEEF};
    os << std::hex << *ns;
    CHECK(os.str() == "deadbeef");
}

TEST_CASE("Buffer Overflow")
{
    auto os = std::ostringstream{};
    const auto* pi = new int[10]{};
    os << pi[10];
    CHECK(os.str() != "7");
    delete pi;
}

TEST_SUITE_END();
