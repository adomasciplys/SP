#include "printer.hpp"

#include "reactant.hpp"
#include "reactant_list.hpp"
#include "reaction.hpp"
#include "symbol_table.hpp"
#include "vessel.hpp"

#include <cstddef>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace stochastic {

// PIMPL
struct Printer::Impl
{
    std::ofstream file;                                // owned when constructed from a path; empty otherwise
    std::ostream& os;                                  // binds to file or the caller's ostream
    SymbolTable<std::string, std::size_t> species_id;  // species name -> s<i> id
    std::size_t reaction_counter{0};                   // next r<i> id

    // ostream branch: no file, just adopt the caller's stream.
    explicit Impl(std::ostream& target) : os(target) {}

    // Path branch: create the parent dir if missing, then open the file.
    explicit Impl(const std::filesystem::path& output_path)
        : os(file)
    {
        if (auto parent = output_path.parent_path(); !parent.empty())
            std::filesystem::create_directories(parent);
        file.open(output_path);
        if (!file)
            throw std::runtime_error("Printer: cannot open " + output_path.string());
    }
};

// Constructor overload for ostream
Printer::Printer(std::ostream& os)
    : _impl(std::make_unique<Impl>(os)) {}

// Constructor overload for filesystem path
Printer::Printer(const std::filesystem::path& output_path)
    : _impl(std::make_unique<Impl>(output_path)) {}

// Defined here, where Impl is complete, so the unique_ptr destructor can compile.
Printer::~Printer() = default;

// Print Reactant, just looks up name in symbol table
void Printer::visit(const Reactant& r)
{
    if (r.is_environment())
        return;  // ø has no node in the graph
    _impl->os << "  s" << _impl->species_id.lookup(r.name)
              << "[label=\"" << r.name
              << "\",shape=\"box\",style=\"filled\",fillcolor=\"cyan\"];\n";
}

// Print Reaction
// Skips catalysts on product side
void Printer::visit(const Reaction& r)
{
    const auto rid = _impl->reaction_counter++;
    _impl->os << "  r" << rid
              << "[label=\"" << r.rate
              << "\",shape=\"oval\",style=\"filled\",fillcolor=\"yellow\"];\n";

    // Inputs: species -> reaction. Catalysts use arrowhead="tee".
    for (const auto& in : r.inputs.items) {
        if (in.is_environment())
            continue;
        _impl->os << "  s" << _impl->species_id.lookup(in.name) << " -> r" << rid;
        if (r.is_catalyst(in.name))
            _impl->os << " [arrowhead=\"tee\"]";
        _impl->os << ";\n";
    }

    // Products: reaction -> species. Catalysts were already drawn as a single
    // tee-arrow input edge above, so skip them here to avoid a redundant edge.
    for (const auto& out : r.products.items) {
        if (out.is_environment())
            continue;
        if (r.is_catalyst(out.name))
            continue;
        _impl->os << "  r" << rid << " -> s" << _impl->species_id.lookup(out.name) << ";\n";
    }
}

// Print the entire reaction network
// Loops over species and reactions and passes in the Printer
void Printer::visit(const Vessel& v)
{
    _impl->os << "digraph {\n";

    // Build the species -> s<i> index in insertion order.
    for (std::size_t i = 0; i < v.species().size(); ++i) {
        const auto& s = v.species()[i];
        if (!s.is_environment())
            _impl->species_id.insert(s.name, i);
    }

    for (const auto& s : v.species())
        s.accept(*this);

    for (const auto& r : v.reactions())
        r.accept(*this);

    _impl->os << "}\n";
    _impl->os.flush();  // ensure the dot output is on disk. Experiences issues during testing without this
}

}  // namespace stochastic
