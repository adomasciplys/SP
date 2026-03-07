#include "term.hpp"
#include "evaluator.hpp"

namespace calculator
{
    double term_t::operator()(state_t& s) const
    {
        Evaluator evaluator{0.0, s};
        accept(evaluator);
        return evaluator.result();
    }
}