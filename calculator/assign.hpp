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
        double operator()(state_t& s) const override;
        void accept(Visitor& v) const override;
        assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> val, const op_t op);

    private:
        std::shared_ptr<var_t> var;
        std::shared_ptr<term_t> term;
        op_t op;
        friend struct Printer;
    };
}

#endif // CALCULATOR_ASSIGN_HPP

