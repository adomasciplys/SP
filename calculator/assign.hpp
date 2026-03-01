#ifndef CALCULATOR_ASSIGN_HPP
#define CALCULATOR_ASSIGN_HPP

#include "term.hpp"
#include "visitor.hpp"
#include "op.hpp"
#include <memory>

namespace calculator
{
    class var_t;

    /** Struct representing assignments */
    struct assign_t : term_t
    {
        void accept(Visitor& v) const override;
        assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> val, const op_t op);
        const std::shared_ptr<var_t> var;
        const std::shared_ptr<term_t> term;
        const op_t op;
    };
}

#endif // CALCULATOR_ASSIGN_HPP
