#include "printer.hpp"
#include "const.hpp"
#include "var.hpp"
#include "unary.hpp"
#include "binary.hpp"
#include "assign.hpp"
#include "expr.hpp"

namespace calculator
{
    Printer::Printer(std::ostream& os, const symbol_table_t& symbols)
        : os{os}, symbols{symbols} {}

    void Printer::visit(const const_t& c) {
        os << c.value();
    }

    void Printer::visit(const var_t& v) {
        os << symbols.names[v.id()];
    }

    void Printer::visit(const unary_t& u) {
        if (u._op == op_t::minus) {
            os << "-";
        } else if (u._op == op_t::plus) {
            os << "+";
        }
        u.operand()->accept(*this);
    }

    void Printer::visit(const binary_t& b) {
        os << "(";
        b.term1()->accept(*this);
        switch (b.op()) {
        case op_t::add: os << " + "; break;
        case op_t::sub: os << " - "; break;
        case op_t::mult: os << " * "; break;
        case op_t::div: os << " / "; break;
        default: break;
        }
        b.term2()->accept(*this);
        os << ")";
    }

    void Printer::visit(const assign_t& a) {
        a.var()->accept(*this);
        switch (a.op()) {
        case op_t::assign: os << " <<= "; break;
        case op_t::add: os << " += "; break;
        case op_t::sub: os << " -= "; break;
        case op_t::mult: os << " *= "; break;
        case op_t::div: os << " /= "; break;
        default: break;
        }
        a.term()->accept(*this);
    }

    void Printer::visit(const expr_t& e) {
        e.term->accept(*this);
    }

    std::ostream& operator<<(std::ostream& os, const PrintTerm& pt) {
        Printer printer{os, pt.symbols};
        pt.term.accept(printer);
        return os;
    }
}

