#ifndef CALCULATOR_VAR_HPP
#define CALCULATOR_VAR_HPP

#include "term.hpp"
#include "visitor.hpp"

namespace calculator
{
    struct expr_t;

    /** Class representing a variable */
    // Since var_t is a class I need to use public
    class var_t : public term_t
    {
        size_t _id; ///< stores the variable identifier
        /** only friends are allowed to construct variable instances */
        explicit var_t(size_t id);

    public:
        var_t(const var_t&) = default;
        var_t& operator=(const var_t&) = default;
        // Keep term_t::operator()(state_t&) visible alongside assignment operator().
        using term_t::operator(); // call the inherited visitor-based evaluator
        /** evaluates an assignment to a  given expression and returns the resulting value */
        double operator()(state_t&, const expr_t&) const;
        friend class symbol_table_t;
        friend struct assign_t;
        void accept(Visitor& v) const override;
        friend struct Printer;
        size_t id () const;
    };
}

#endif // CALCULATOR_VAR_HPP
