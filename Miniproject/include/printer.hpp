#ifndef MINIPROJECT_PRINTER_HPP
#define MINIPROJECT_PRINTER_HPP

#include "symbol_table.hpp"
#include "visitor.hpp"

#include <cstddef>
#include <ostream>
#include <string>

namespace stochastic {

struct Reactant;
struct Reaction;
struct Vessel;

// A Visitor that emits the reaction network in Graphviz `dot` format.
// Species become cyan boxes (s0, s1, ...)
// Reactions become yellow ovals (r0, r1, ...) labelled with their rate.
// Catalysts (species that appear on both sides of a reaction) get an `arrowhead="tee"`
// edge instead of a pair of in/out edge
//
// Usage:
//     Printer{std::cout}.visit(vessel);
//
struct Printer : Visitor
{
    explicit Printer(std::ostream& os);

    void visit(const Reactant& r) override;
    void visit(const Reaction& r) override;
    void visit(const Vessel& v) override;

private:
    std::ostream& _os;
    SymbolTable<std::string, std::size_t> _species_id;  // species name -> s<i> id
    std::size_t _reaction_counter{0};                   // next r<i> id
};

}  // namespace stochastic

#endif  // MINIPROJECT_PRINTER_HPP
