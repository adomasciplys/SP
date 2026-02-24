#ifndef INCLUDE_ALGEBRA_HPP
#define INCLUDE_ALGEBRA_HPP

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include "calculator.hpp"

namespace calculator
{
    /** Type to capture the state of entire calculator (one number per variable): */
    using state_t = std::vector<double>;

    /** Forward declarations to get around circular dependencies: */
    struct expr_t;
    struct var_t;

    /** Enum representing the operations we can do */
    enum op_t {///< operator sign for assignment/unary/binary expression
        plus,  ///< unary plus, like +v
        minus, ///< unary minus, like -v
        add,   ///< binary addition, like v+v
        sub,   ///< binary addition, like v-v
        assign, ///< assignment, like v <<= v
        mult, ///< binary multiplication, like v * v
        div ///< binary division, like v / v
    };

    /** Abstract class as a general interface to all kinds of expressions */
    struct term_t
    {
        term_t();
        virtual ~term_t() noexcept = default;
        virtual double operator()(state_t&) const = 0;
    };

    /** Class representing a constant value expression */
    struct const_t : term_t
    {
        double operator()(state_t&) const {return value;}
        private:
            double value;
            explicit const_t(double value): value{value} {}
    };

    /** Class representing unary expressions */
    struct unary_t : term_t
    {
        unary_t(std::shared_ptr<term_t> operand, const op_t op) : operand{std::move(operand)}, op{op} {}

        double operator()(state_t& s) const override
        {
            if (!operand)
                throw std::logic_error{"missing operand for unary operation"};

            switch(op) {
            case op_t::plus:
                return operand->operator()(s);
            case op_t::minus:
                return -operand->operator()(s);
            default:
                throw std::logic_error{"unsupported unary operation"};
            }
        }

        private:
            std::shared_ptr<term_t> operand;
            op_t op;
    };

    /** Struct representing binary expressions */
    struct binary_t : term_t
    {
        binary_t(std::shared_ptr<term_t> term1, std::shared_ptr<term_t> term2, const op_t op)
            : term1{std::move(term1)}, term2{std::move(term2)}, op{op} {}

        double operator()(state_t& s) const override
        {
            if (!term1)
                throw std::logic_error{"missing term 1"};
            if (!term2)
                throw std::logic_error{"missing term 2"};

            double val1 = term1->operator()(s);
            double val2 = term2->operator()(s);

            switch(op)
            {
            case op_t::add:
                return val1 + val2;
            case op_t::sub:
                return val1 - val2;
            case op_t::mult:
                return val1 * val2;
            case op_t::div:
                if (val2 == 0.0)
                    throw std::logic_error{"division by zero"};
                return val1 / val2;
            default:
                throw std::logic_error{"unsupported binary operation"};
            }

        }

        private:
            std::shared_ptr<term_t> term1, term2;
            op_t op;

    };


    /** Class representing a variable */
    class var_t : term_t
    {
        size_t id; ///< stores the variable identifier
        /** only friends are allowed to construct variable instances */
        explicit var_t(size_t id): id{id} {}
    public:
        var_t(const var_t&) = default;
        var_t& operator=(const var_t&) = default;
        /** returns the value of the variable stored in a state */
        double operator()(state_t& s) const override { return s[id]; }
        /** evaluates an assignment to a given expression and returns the resulting value */
        double operator()(state_t&, const expr_t&) const;
        friend class symbol_table_t;
        friend struct assign_t;
    };

    /** Class representing assignments */
    struct assign_t : term_t
    {
        assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> val, const op_t op)
            : var{std::move(var)}, term{std::move(val)}, op{op} {}

        double operator()(state_t& s) const override
        {
            if (!var)
                throw std::logic_error{"missing variable"};
            if (!term)
                throw std::logic_error{"missing term"};
            return s[var->id] = term->operator()(s);

        }
    private:
        std::shared_ptr<var_t> var;
        std::shared_ptr<term_t> term;
        op_t op;
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
        op_t op;
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
                    case op_t::plus: return v(s);
                    case op_t::minus: return -v(s);
                    case op_t::assign:
                        if (operands.empty())
                            throw std::logic_error{"missing expression to evaluate"};
                        return v(s, *operands[0]);
                    default:
                        throw std::logic_error{"unsupported operation over a variable"};
                }
            } else { // more complicated expression with operands
                auto& e1 = *operands[0];
                switch(op) {
                case op_t::plus: // unary plus
                        if (operands.empty())
                            throw std::logic_error{"bug: missing an operand for unary +"};
                        return e1(s);
                    case op_t::minus: // unary minus
                        if (operands.empty())
                            throw std::logic_error{"bug: missing an operand for unary -"};
                        return -e1(s);
                    case op_t::add: { // binary addition
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary +"};
                        auto &e2 = *operands[1];
                        return e1(s) + e2(s);
                    }
                    case op_t::sub: { // binary subtraction
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary -"};
                        auto &e2 = *operands[1];
                        return e1(s) - e2(s);
                    }
                    case op_t::mult: {
                            if (operands.size() != 2)
                                throw std::logic_error{"bug: expecting two operands for binary -"};
                            auto &e2 = *operands[1];
                            return e1(s) * e2(s);
                    }
                    case op_t::div: {
                            if (operands.size() != 2)
                                throw std::logic_error{"bug: expecting two operands for binary -"};
                            auto &e2 = *operands[1];
                            // Check if the expression evaluates to 0
                            if (e2(s) == 0)
                            {
                                throw std::logic_error{"division by zero"};
                            }
                            return e1(s) / e2(s);
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
    inline expr_t operator+(const expr_t& e) { return expr_t{e, op_t::plus}; }
    inline expr_t operator-(const expr_t& e) { return expr_t{e, op_t::minus}; }

    /** binary operators: */
    inline expr_t operator+(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::add}; }
    inline expr_t operator-(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::sub}; }
    inline expr_t operator<<=(const var_t& v, const expr_t& e) { return expr_t{v, e}; }

    /// TODO: implement multiplication
    inline expr_t operator*(const expr_t& e1, const expr_t& e2) { return expr_t{e1,e2, op_t::mult}; }
    inline expr_t operator/(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::div}; }

    /// TODO: refactor expr_t into AST terms
    /// TODO: add support for constant expressions like: 7
    /// TODO: add support for printing
}

#endif // INCLUDE_ALGEBRA_HPP
