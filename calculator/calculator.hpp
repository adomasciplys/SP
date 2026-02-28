#ifndef INCLUDE_ALGEBRA_HPP
#define INCLUDE_ALGEBRA_HPP

#include "op.hpp"
#include "term.hpp"
#include "visitor.hpp"
#include "const.hpp"
#include "unary.hpp"
#include "binary.hpp"
#include "var.hpp"
#include "assign.hpp"
#include "expr.hpp"
#include "symbol_table.hpp"
#include "printer.hpp"

namespace calculator
{
    /** assignment operation */
    inline double var_t::operator()(state_t& s, const expr_t& e) const { return s[id] = e(s); }

    /** unary operators: */
    inline expr_t operator+(const expr_t& e) { return expr_t{e, op_t::plus}; }
    inline expr_t operator-(const expr_t& e) { return expr_t{e, op_t::minus}; }

    /** binary operators: */
    inline expr_t operator+(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::add}; }
    inline expr_t operator-(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::sub}; }
    inline expr_t operator<<=(const var_t& v, const expr_t& e) { return expr_t{v, e}; }

    inline expr_t operator+=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::add}; }
    inline expr_t operator-=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::sub}; }
    inline expr_t operator*=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::mult}; }
    inline expr_t operator/=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::div}; }

    inline expr_t operator*(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::mult}; }
    inline expr_t operator/(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::div}; }
}

#endif // INCLUDE_ALGEBRA_HPP
