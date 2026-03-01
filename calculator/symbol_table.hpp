#ifndef CALCULATOR_SYMBOL_TABLE_HPP
#define CALCULATOR_SYMBOL_TABLE_HPP

#include "term.hpp"
#include "var.hpp"
#include <vector>
#include <string>

namespace calculator
{
    /** Symbol table - UNMODIFIED */
    class symbol_table_t
    {
        std::vector<std::string> names; ///< stores the variable names
        std::vector<double> initial; ///< stores the initial values of variables
    public:
        /// Creates a variable with given name and initial value
        [[nodiscard]] var_t var(std::string name, double init = 0);

        /// Creates a system state initialized with initial variable values
        [[nodiscard]] state_t state() const;

        /// Returns variable name for a given variable id
        [[nodiscard]] const std::string& name(size_t id) const;
    };
}

#endif // CALCULATOR_SYMBOL_TABLE_HPP
