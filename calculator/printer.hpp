#ifndef CALCULATOR_PRINTER_HPP
#define CALCULATOR_PRINTER_HPP

#include "visitor.hpp"
#include "symbol_table.hpp"
#include <ostream>

namespace calculator
{
    struct const_t;
    struct unary_t;
    struct binary_t;
    struct assign_t;
    struct expr_t;
    class var_t;

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

#endif // CALCULATOR_PRINTER_HPP

