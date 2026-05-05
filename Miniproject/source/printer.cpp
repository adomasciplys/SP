#include "printer.hpp"

#include "reactant.hpp"
#include "reactant_list.hpp"
#include "reaction.hpp"
#include "vessel.hpp"

namespace stochastic {

namespace {
bool contains_named(const ReactantList& list, const std::string& name)
{
    for (const auto& r : list.items)
        if (r.name == name)
            return true;
    return false;
}
}  // namespace

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

    // Inputs: species -> reaction. Catalysts (also in products) use arrowhead="tee"
    // and skip the matching reaction -> species edge below.
    for (const auto& in : r.inputs.items) {
        if (in.is_environment())
            continue;
        const bool is_catalyst = contains_named(r.products, in.name);
        _os << "  s" << _species_id.lookup(in.name) << " -> r" << rid;
        if (is_catalyst)
            _os << " [arrowhead=\"tee\"]";
        _os << ";\n";
    }

    // Products: reaction -> species, skipping catalysts already drawn above.
    for (const auto& out : r.products.items) {
        if (out.is_environment())
            continue;
        if (contains_named(r.inputs, out.name))
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
