#ifndef MINIPROJECT_PRINTER_HPP
#define MINIPROJECT_PRINTER_HPP

#include "symbol_table.hpp"
#include "visitor.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>
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
//     Printer{"figures/seihr.dot"}.visit(vessel);   // write to a file
//     Printer{os}.visit(vessel);                    // write to any ostream (tests)
//
struct Printer : Visitor
{
    // Write the dot output to an existing ostream (used by tests with std::ostringstream).
    explicit Printer(std::ostream& os);

    // Write the dot output to `output_path`. The parent directory is created if missing.
    // Throws std::runtime_error if the file cannot be opened.
    explicit Printer(const std::filesystem::path& output_path);

    void visit(const Reactant& r) override;
    void visit(const Reaction& r) override;
    void visit(const Vessel& v) override;

private:
    std::ofstream _file;                                // owned when constructed from a path; empty otherwise
    std::ostream& _os;                                  // binds to _file or the caller's ostream
    SymbolTable<std::string, std::size_t> _species_id;  // species name -> s<i> id
    std::size_t _reaction_counter{0};                   // next r<i> id
};

}  // namespace stochastic

#endif  // MINIPROJECT_PRINTER_HPP
