#ifndef CALCULATOR_BINARY_HPP
#define CALCULATOR_BINARY_HPP

#include "term.hpp"
#include "visitor.hpp"
#include "op.hpp"
#include <memory>

namespace calculator
{
    /** Struct representing binary expressions */
    struct binary_t : term_t
    {
        double operator()(state_t& s) const override;
        void accept(Visitor& v) const override;
        binary_t(std::shared_ptr<term_t> term1, std::shared_ptr<term_t> term2, const op_t op);

    private:
        std::shared_ptr<term_t> term1, term2;
        op_t op;
        friend struct Printer;
    };
}

#endif // CALCULATOR_BINARY_HPP

