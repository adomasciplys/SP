#include "factorial.hpp"

#include "doctest.h"

TEST_CASE("Runtime Factorial")
{
	REQUIRE(fact(0) == 1u);
	REQUIRE(fact(1) == 1u);
	REQUIRE(fact(2) == 2u);
	REQUIRE(fact(3) == 6u);
	REQUIRE(fact(4) == 24u);
	REQUIRE(fact(5) == 120u);
	REQUIRE(fact(40) == 18376134811363311616u);
}

/** compile-time Factorial meta-function */
TEST_CASE("Compile-time Factorial")
{
	REQUIRE(Fact<0>::value == 1u);
	REQUIRE(Fact<1>::value == 1u);
	REQUIRE(Fact<2>::value == 2u);
	REQUIRE(Fact<3>::value == 6u);
	REQUIRE(Fact<4>::value == 24u);
	REQUIRE(Fact<5>::value == 120u);
	REQUIRE(Fact<40>::value == 18376134811363311616u);
}
