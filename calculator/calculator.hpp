#ifndef INCLUDE_ALGEBRA_HPP
#define INCLUDE_ALGEBRA_HPP

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace calculator
{
    /** Type to capture the state of entire calculator (one number per variable): */
    using state_t = std::vector<double>;

    /** Forward declarations to get around circular dependencies: */
    struct expr_t;

    /** Class representing a variable */
    class var_t
    {
        size_t id; ///< stores the variable identifier
        /** only friends are allowed to construct variable instances */
        explicit var_t(size_t id): id{id} {}
    public:
        var_t(const var_t&) = default;
        var_t& operator=(const var_t&) = default;
        /** returns the value of the variable stored in a state */
        double operator()(const state_t& s) const { return s[id]; }
        /** evaluates an assignment to a given expression and returns the resulting value */
        double operator()(state_t&, const expr_t&) const;
        friend class symbol_table_t;
    };

    class symbol_table_t
    {
        std::vector<std::string> names; ///< stores the variable names
        std::vector<double> initial;    ///< stores the initial values of variables
    public:
        /// Creates a variable with given name and initial value
        [[nodiscard]] var_t var(std::string name, double init = 0) {
            auto res = names.size();
            names.push_back(std::move(name));
            initial.push_back(init);
            return var_t{res};
        }
        /// Creates a system state initialized with initial variable values
        [[nodiscard]] state_t state() const { return {initial}; }
    };

    /**
     * expr_t represents an expression and implements all operations.
     * TODO: such design does not scale and has to be refactored into distinct AST classes.
     */
    struct expr_t
    {
        std::unique_ptr<var_t> var; ///< stores the variable in case it is a variable expression
        std::vector<std::unique_ptr<expr_t>> operands; ///< stores operands in case it is assignment/unary/binary expression
        enum op_t {///< operator sign for assignment/unary/binary expression
            plus,  ///< unary plus, like +v
            minus, ///< unary minus, like -v
            add,   ///< binary addition, like v+v
            sub,   ///< binary addition, like v-v
            assign ///< assignment, like v <<= v
        } op;
        expr_t(const expr_t& other) { *this = other; }
        expr_t& operator=(const expr_t& other) {
            op = other.op;
            var = other.var ? std::make_unique<var_t>(*other.var) : nullptr;
            operands.resize(other.operands.size());
            std::transform(other.operands.begin(), other.operands.end(), operands.begin(),
                           [](const auto& ptr) { return std::make_unique<expr_t>(*ptr); } );
            return *this;
        }
        expr_t(expr_t&& other) noexcept = default;
        expr_t& operator=(expr_t&& other) noexcept = default;
        ~expr_t() noexcept = default;

        expr_t(const var_t& v, op_t op = plus): var{std::make_unique<var_t>(v)}, op{op} {}
        expr_t(const var_t& v, const expr_t& e): var{std::make_unique<var_t>(v)}, operands(1), op{assign} {
            operands[0] = std::make_unique<expr_t>(e);
        }
        expr_t(const expr_t& e, op_t op): operands(1), op{op} {
            operands[0] = std::make_unique<expr_t>(e);
        }
        expr_t(const expr_t& e1, const expr_t& e2, op_t op): operands(2), op{op} {
            operands[0] = std::make_unique<expr_t>(e1);
            operands[1] = std::make_unique<expr_t>(e2);
        }

        double operator()(state_t& s) const {
            // TODO: slice this big code into smaller pieces in const_t/var_t/assignt_t/unary_t/binary_t::operator()
            if (var) { // variable expression
                auto& v = *var;
                switch(op) {
                    case expr_t::plus: return v(s);
                    case expr_t::minus: return -v(s);
                    case expr_t::assign:
                        if (operands.empty())
                            throw std::logic_error{"missing expression to evaluate"};
                        return v(s, *operands[0]);
                    default:
                        throw std::logic_error{"unsupported operation over a variable"};
                }
            } else { // more complicated expression with operands
                auto& e1 = *operands[0];
                switch(op) {
                    case expr_t::plus: // unary plus
                        if (operands.empty())
                            throw std::logic_error{"bug: missing an operand for unary +"};
                        return e1(s);
                    case expr_t::minus: // unary minus
                        if (operands.empty())
                            throw std::logic_error{"bug: missing an operand for unary -"};
                        return -e1(s);
                    case expr_t::add: { // binary addition
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary +"};
                        auto &e2 = *operands[1];
                        return e1(s) + e2(s);
                    }
                    case expr_t::sub: { // binary subtraction
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary -"};
                        auto &e2 = *operands[1];
                        return e1(s) - e2(s);
                    }
                    default:
                        throw std::logic_error{"not implemented"};
                }
            }
        }
    };

    /** assignment operation */
    inline double var_t::operator()(state_t& s, const expr_t& e) const { return s[id] = e(s); }

    /** unary operators: */
    inline expr_t operator+(const expr_t& e) { return expr_t{e, expr_t::plus}; }
    inline expr_t operator-(const expr_t& e) { return expr_t{e, expr_t::minus}; }

    /** binary operators: */
    inline expr_t operator+(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, expr_t::add}; }
    inline expr_t operator-(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, expr_t::sub}; }
    inline expr_t operator<<=(const var_t& v, const expr_t& e) { return expr_t{v, e}; }

    /// TODO: implement multiplication
    /// TODO: refactor expr_t into AST terms
    /// TODO: add support for constant expressions like: 7
    /// TODO: add support for printing
}

#endif // INCLUDE_ALGEBRA_HPP
