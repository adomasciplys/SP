#ifndef MINIPROJECT_PRINTER_HPP
#define MINIPROJECT_PRINTER_HPP

#include "visitor.hpp"

#include <filesystem>
#include <iosfwd>
#include <memory>

namespace stochastic {

struct Reactant;
struct Reaction;
struct Vessel;

// A Visitor that emits the reaction network in Graphviz `dot` format.
// Species become cyan boxes (s0, s1, ...)
// Reactions become yellow ovals (r0, r1, ...) labelled with their rate.
// Catalysts get a special edge (arrowhead="tee")
//
// Usage:
//     Printer{"figures/seihr.dot"}.visit(vessel);   // write to a file
//     Printer{os}.visit(vessel);                    // write to any ostream
//
// I have tried to use PIMPL here
struct Printer : Visitor
{
    // Write the dot output to an existing ostream
    explicit Printer(std::ostream& os);

    // Write the dot output to output_path. The parent directory is created if missing.
    explicit Printer(const std::filesystem::path& output_path);

    // I need to declare destrutor otherwise the header will not compile
    // due to forward declaration of struct Impl
    ~Printer() override;

    // Since this struct will handle files streams I would like to avoid having it write to the same file
    Printer(const Printer&) = delete;
    Printer& operator=(const Printer&) = delete;

    // Move is also deleted just to signal that the Printer should only be used once and then destructed
    Printer(Printer&&) = delete;
    Printer& operator=(Printer&&) = delete;

    void visit(const Reactant& r) override;
    void visit(const Reaction& r) override;
    void visit(const Vessel& v) override;

private:
    struct Impl;                    // defined in printer.cpp
    std::unique_ptr<Impl> _impl;
};

}  // namespace stochastic

#endif  // MINIPROJECT_PRINTER_HPP
