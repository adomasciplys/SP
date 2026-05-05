#include "printer.hpp"

#include "reactant.hpp"
#include "reactant_list.hpp"
#include "reaction.hpp"
#include "vessel.hpp"

#include <algorithm>

namespace stochastic {

static bool contains(const ReactantList& list, const std::string& name)
{
    return std::ranges::any_of(list.items,
                               [&](const Reactant& r) { return r.name == name; });
}

// A species is a catalyst of a reaction when it appears on both sides:
// consumed as an input and produced again, with no net change in count.
static bool is_catalyst(const Reaction& r, const std::string& name)
{
    return contains(r.inputs, name) && contains(r.products, name);
}

Printer::Printer(std::ostream& os) : _os(os) {}

void Printer::visit(const Reactant& r)
{
    if (r.is_environment())
        return;  // ∅ has no node in the graph
    _os << "  s" << _species_id.lookup(r.name)
        << "[label=\"" << r.name
        << "\",shape=\"box\",style=\"filled\",fillcolor=\"cyan\"];\n";
}

void Printer::visit(const Reaction& r)
{
    const auto rid = _reaction_counter++;
    _os << "  r" << rid
        << "[label=\"" << r.rate
        << "\",shape=\"oval\",style=\"filled\",fillcolor=\"yellow\"];\n";

    // Inputs: species -> reaction. Catalysts use arrowhead="tee".
    for (const auto& in : r.inputs.items) {
        if (in.is_environment())
            continue;
        _os << "  s" << _species_id.lookup(in.name) << " -> r" << rid;
        if (is_catalyst(r, in.name))
            _os << " [arrowhead=\"tee\"]";
        _os << ";\n";
    }

    // Products: reaction -> species. Catalysts were already drawn as a single
    // tee-arrow input edge above, so skip them here to avoid a redundant edge.
    for (const auto& out : r.products.items) {
        if (out.is_environment())
            continue;
        if (is_catalyst(r, out.name))
            continue;
        _os << "  r" << rid << " -> s" << _species_id.lookup(out.name) << ";\n";
    }
}

void Printer::visit(const Vessel& v)
{
    _os << "digraph {\n";

    // Build the species -> s<i> index in insertion order.
    for (std::size_t i = 0; i < v.species().size(); ++i) {
        const auto& s = v.species()[i];
        if (!s.is_environment())
            _species_id.insert(s.name, i);
    }

    for (const auto& s : v.species())
        visit(s);

    for (const auto& r : v.reactions())
        visit(r);

    _os << "}\n";
}

}  // namespace stochastic
