#include "binary.hpp"

namespace calculator
{
    binary_t::binary_t(std::shared_ptr<term_t> term1, std::shared_ptr<term_t> term2, const op_t op)
        : term1{std::move(term1)}, term2{std::move(term2)}, op{op}
    {
    }

    void binary_t::accept(Visitor& v) const { v.visit(*this); }
}
