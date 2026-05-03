#ifndef MINIPROJECT_PRINTER_HPP
#define MINIPROJECT_PRINTER_HPP

#include <ostream>
#include <visitor.hpp>

namespace stochastic
{
    struct PartialReaction;
    struct Reactant;
    struct ReactantList;
    struct Reaction;
    struct Vessel;

    struct Printer : Visitor
    {
        std::ostream& os;
        const symbol_table_t& symbols;

        Printer(std::ostream& os, const symbol_table_t& symbols);

        void visit(const const_t& c) override;
        void visit(const var_t& v) override;
        void visit(const unary_t& u) override;
        void visit(const binary_t& b) override;
        void visit(const assign_t& a) override;
        void visit(const expr_t& e) override;
    };

    struct PrintTerm {
        const term_t& term;
        const symbol_table_t& symbols;
    };

    std::ostream& operator<<(std::ostream& os, const PrintTerm& pt);
}

#endif //MINIPROJECT_PRINTER_HPP