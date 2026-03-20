#include "abc.hpp"
#include "abc_csv.hpp"

#include <doctest/doctest.h>

#include <sstream>

/// equality operator to support easy doctest
bool operator==(const ABC& s1, const ABC& s2) { return s1.a == s2.a && s1.b == s2.b && s1.c == s2.c; }
/// output operator to support humane doctest output
std::ostream& operator<<(std::ostream& os, const ABC& other)
{
    return os << "ABC{" << other.a << ", " << other.b << ", " << other.c << "}";
}

TEST_CASE("ABC")
{
    // Dummy test to check doctest support
    auto os = std::ostringstream{};
    os << ABC{1, 2, 3};
    CHECK(os.str() == "ABC{1, 2, 3}");    // doctest relies on operator<< to pretty print failed CHECKs
    CHECK(ABC{1, 2, 3} == ABC{1, 2, 3});  // should pass
    // CHECK(ABC{1,2,3} == ABC{2,3,4}); // should fail
}

TEST_CASE("read_abc")
{
    // TODO: focus on these tests
    // R"raw()raw" stands for raw string in between `raw(` and `)raw`
    auto is = std::istringstream(R"raw(1, 2, 3
 4, 5, 6
7 ,8 ,9
 10 , 11 , 12
)raw");
    auto data = read_abc(is);
    REQUIRE(data.size() == 4);
    REQUIRE(data[0] == ABC{1, 2, 3});
    REQUIRE(data[1] == ABC{4, 5, 6});
    REQUIRE(data[2] == ABC{7, 8, 9});
    REQUIRE(data[3] == ABC{10, 11, 12});
}