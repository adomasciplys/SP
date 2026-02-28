#include "symbol_table.hpp"

namespace calculator
{
    /// Creates a variable with given name and initial value
    var_t symbol_table_t::var(std::string name, double init)
    {
        auto res = names.size();
        names.push_back(std::move(name));
        initial.push_back(init);
        return var_t{res};
    }

    /// Creates a system state initialized with initial variable values
    state_t symbol_table_t::state() const { return {initial}; }
}

