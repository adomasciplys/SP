#ifndef CALCULATOR_TERM_HPP
#define CALCULATOR_TERM_HPP

#include <vector>

namespace calculator
{
    /** Type to capture the state of entire calculator (one number per variable): */
    using state_t = std::vector<double>;

    struct Visitor;

    /** Abstract class as a general interface to all kinds of expressions */
    struct term_t
    {
        // Ensure proper cleanup of deriving classes
        virtual ~term_t() noexcept = default;
        // All deriving classes must implement the operator method
        virtual double operator()(state_t&) const = 0;
        virtual void accept(Visitor& v) const = 0;
    };
}

#endif // CALCULATOR_TERM_HPP

