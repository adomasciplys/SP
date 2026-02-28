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
        friend struct Printer;
    public:
        /// Creates a variable with given name and initial value
        [[nodiscard]] var_t var(std::string name, double init = 0);

        /// Creates a system state initialized with initial variable values
        [[nodiscard]] state_t state() const;
    };
}

#endif // CALCULATOR_SYMBOL_TABLE_HPP

