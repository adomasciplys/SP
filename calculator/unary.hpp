#ifndef CALCULATOR_UNARY_HPP
#define CALCULATOR_UNARY_HPP

#include "term.hpp"
#include "visitor.hpp"
#include "op.hpp"
#include <memory>

namespace calculator
{
    /** Struct representing unary expressions */
    struct unary_t : term_t
    {
        // Return value of term with operator applied to it
        void accept(Visitor& v) const override;
        unary_t(std::shared_ptr<term_t> operand, const op_t op);
        std::shared_ptr<term_t> operand() const;
        op_t op() const;

    private:
        std::shared_ptr<term_t> _operand;
        op_t _op;
        friend struct Printer;
    };
}

#endif // CALCULATOR_UNARY_HPP

