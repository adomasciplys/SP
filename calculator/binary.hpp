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
        void accept(Visitor& v) const override;
        binary_t(std::shared_ptr<term_t> term1, std::shared_ptr<term_t> term2, const op_t op);
        std::shared_ptr<term_t> term1() const;
        std::shared_ptr<term_t> term2() const;
        op_t op() const;

    private:
        std::shared_ptr<term_t> _term1, _term2;
        op_t _op;
        friend struct Printer;
    };
}

#endif // CALCULATOR_BINARY_HPP

