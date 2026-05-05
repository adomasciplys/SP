#include "printer.hpp"
#include "vessel.hpp"

#include <doctest/doctest.h>

#include <sstream>
#include <string>

using stochastic::Printer;
using stochastic::Vessel;

namespace {
std::string dot_of(const Vessel& v)
{
    std::ostringstream os;
    Printer{os}.visit(v);
    return os.str();
}
}  // namespace

TEST_CASE("Printer: empty vessel emits an empty digraph")
{
    Vessel v{"empty"};
    const auto out = dot_of(v);
    CHECK(out == "digraph {\n}\n");
}

TEST_CASE("Printer: single species, no reactions")
{
    Vessel v{"one"};
    v.add("A", 0);
    const auto out = dot_of(v);
    CHECK(out.find("s0[label=\"A\",shape=\"box\",style=\"filled\",fillcolor=\"cyan\"];")
          != std::string::npos);
}

TEST_CASE("Printer: simple reaction A >> 1.0 >>= B")
{
    Vessel v{"AB"};
    const auto A = v.add("A", 1);
    const auto B = v.add("B", 0);
    v.add(A >> 1.0 >>= B);

    const auto out = dot_of(v);
    CHECK(out.find("s0 -> r0;") != std::string::npos);
    CHECK(out.find("r0 -> s1;") != std::string::npos);
    // Plain edge — no tee on a non-catalyst input.
    CHECK(out.find("arrowhead=\"tee\"") == std::string::npos);
}

TEST_CASE("Printer: catalyst (S+I >>= E+I) emits a tee-arrow and no redundant edge")
{
    Vessel v{"catalyst"};
    const auto S = v.add("S", 100);
    const auto E = v.add("E", 0);
    const auto I = v.add("I", 1);
    v.add((S + I) >> 0.5 >>= E + I);

    const auto out = dot_of(v);
    // I is a catalyst -> tee on the input edge.
    CHECK(out.find("s2 -> r0 [arrowhead=\"tee\"];") != std::string::npos);
    // S is a normal input.
    CHECK(out.find("s0 -> r0;") != std::string::npos);
    // E is the only real product.
    CHECK(out.find("r0 -> s1;") != std::string::npos);
    // No redundant r0 -> s2 edge for the catalyst.
    CHECK(out.find("r0 -> s2") == std::string::npos);
}

TEST_CASE("Printer: decay reaction A >>= env emits an input edge and no product edge")
{
    Vessel v{"decay"};
    const auto env = v.environment();
    const auto A = v.add("A", 5);
    v.add(A >> 0.1 >>= env);

    const auto out = dot_of(v);
    CHECK(out.find("s0 -> r0;") != std::string::npos);
    // No edge from r0 to anything: ∅ has no node.
    CHECK(out.find("r0 -> s") == std::string::npos);
}
