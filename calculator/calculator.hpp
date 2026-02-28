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
    class var_t;
    struct term_t;
    struct const_t;
    struct unary_t;
    struct binary_t;
    struct assign_t;

    /** Enum representing the operations we can do */

    enum op_t
    {
        ///< operator sign for assignment/unary/binary expression
        plus, ///< unary plus, like +v
        minus, ///< unary minus, like -v
        add, ///< binary addition, like v+v
        sub, ///< binary addition, like v-v
        assign, ///< assignment, like v <<= v
        mult, ///< binary multiplication, like v * v
        div ///< binary division, like v / v
    };

    /** Small helper to check for division by 0 */
    inline double safe_divide(const double a, const double b) {
        if (b == 0.0)
            throw std::logic_error{"division by zero"};
        return a / b;
    }

    struct Visitor
    {
        virtual void visit(const expr_t&) = 0;
        virtual void visit(const var_t&) = 0;
        virtual void visit(const const_t&) = 0;
        virtual void visit(const unary_t&) = 0;
        virtual void visit(const binary_t&) = 0;
        virtual void visit(const assign_t&) = 0;
        virtual ~Visitor() noexcept = default;

    };

    /** Abstract class as a general interface to all kinds of expressions */

    struct term_t
    {
        // Ensure proper cleanup of deriving classes
        virtual ~term_t() noexcept = default;
        // All deriving classes must implement the operator method
        virtual double operator()(state_t&) const = 0;
        virtual void accept(Visitor& v) const = 0;
    };

    /** Struct representing a constant value expression */

    struct const_t : term_t // Inherit from term_t
    {
        // Simply returns the value it stores
        double operator()(state_t&) const override { return value; }
        void accept(Visitor& v) const override { v.visit(*this); }
        const_t(double value) : value{value}
        {
        }

    private:
        double value;
        friend struct Printer;
    };

    /** Struct representing unary expressions */
    struct unary_t : term_t // Inherit from term_t
    {
        unary_t(std::shared_ptr<term_t> operand, const op_t op) : operand{std::move(operand)}, op{op}
        {
        }
        // Return value of term with operator applied to it
        double operator()(state_t& s) const override
        {
            if (!operand)
                throw std::logic_error{"missing operand for unary operation"};

            // Select based on operator
            switch (op)
            {
            case plus:
                return operand->operator()(s); // Evaluate expression unchanged
            case minus:
                return -operand->operator()(s); // Evaluate expression and negate
            default:
                throw std::logic_error{"unsupported unary operation"};
            }
        }
        void accept(Visitor& v) const override { v.visit(*this); }

    private:
        std::shared_ptr<term_t> operand;
        op_t op;
        friend struct Printer;
    };

    /** Struct representing binary expressions */

    struct binary_t : term_t
    {
        binary_t(std::shared_ptr<term_t> term1, std::shared_ptr<term_t> term2, const op_t op)
            : term1{std::move(term1)}, term2{std::move(term2)}, op{op}
        {
        }

        double operator()(state_t& s) const override
        {
            if (!term1)
                throw std::logic_error{"missing term 1"};
            if (!term2)
                throw std::logic_error{"missing term 2"};

            double val1 = term1->operator()(s); // Evaluate first term
            double val2 = term2->operator()(s); // Evaluate second term

            switch (op)
            {
            case add:
                return val1 + val2;
            case sub:
                return val1 - val2;
            case mult:
                return val1 * val2;
            case div:
                return safe_divide(val1, val2);
            default:
                throw std::logic_error{"unsupported binary operation"};
            }
        }
        void accept(Visitor& v) const override { v.visit(*this); }

    private:
        std::shared_ptr<term_t> term1, term2;
        op_t op;
        friend struct Printer;
    };


    /** Class representing a variable */

    // Since var_t is a class I need to use public
    class var_t : public term_t
    {
        size_t id; ///< stores the variable identifier
        /** only friends are allowed to construct variable instances */
        explicit var_t(size_t id) : id{id}
        {
        }

    public:
        var_t(const var_t&) = default;
        var_t& operator=(const var_t&) = default;
        /** returns the value of the variable stored in a state */
        double operator()(state_t& s) const override { return s[id]; }
        /** evaluates an assignment to a given expression and returns the resulting value */
        double operator()(state_t&, const expr_t&) const;
        friend class symbol_table_t;
        friend struct assign_t;
        void accept(Visitor& v) const override { v.visit(*this); }
        friend struct Printer;
    };

    /** Struct representing assignments */

    struct assign_t : term_t
    {
        assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> val, const op_t op)
            : var{std::move(var)}, term{std::move(val)}, op{op}
        {
        }

        double operator()(state_t& s) const override
        {
            if (!var)
                throw std::logic_error{"missing variable"};
            if (!term)
                throw std::logic_error{"missing term"};

            const double rhs = term->operator()(s); // Evaluate rhs
            // Get a reference to var in state where we want to store the new value
            double& lhs = s[var->id];

            switch (op)
            {
            case assign:
                return lhs = rhs;
            case add:
                return lhs = lhs + rhs; // +=
            case sub:
                return lhs = lhs - rhs; // -=
            case mult:
                return lhs = lhs * rhs; // *=
            case div:
                return lhs = safe_divide(lhs, rhs); // /=
            default:
                throw std::logic_error{"unsupported assignment operation"};
            }
        }
        void accept(Visitor& v) const override { v.visit(*this); }

    private:
        std::shared_ptr<var_t> var;
        std::shared_ptr<term_t> term;
        op_t op;
        friend struct Printer;
    };

    /** Symbol table - UNMODIFIED */

    class symbol_table_t
    {
        std::vector<std::string> names; ///< stores the variable names
        std::vector<double> initial; ///< stores the initial values of variables
        friend struct Printer;
    public:
        /// Creates a variable with given name and initial value
        [[nodiscard]] var_t var(std::string name, double init = 0)
        {
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
     */
    struct expr_t
    {
        std::shared_ptr<term_t> term;
        friend struct Printer;
        // If we get a term we simply move
        expr_t(std::shared_ptr<term_t> term) : term{std::move(term)}
        {
        }

        // Implicit conversion from var_t
        expr_t(const var_t& v) : term{std::make_shared<var_t>(v)}
        {
        }

        // Implicit conversion from double
        expr_t(double value) : term{std::make_shared<const_t>(value)}
        {
        }

        // Constructor for unary operations
        expr_t(const expr_t& e, op_t op)
            : term{std::make_shared<unary_t>(e.term, op)}
        {
        }

        // Constructor for binary operations
        expr_t(const expr_t& e1, const expr_t& e2, op_t op)
            : term{std::make_shared<binary_t>(e1.term, e2.term, op)}
        {
        }

        // Constructor for assignment operations
        expr_t(const var_t& v, const expr_t& e)
            : term{std::make_shared<assign_t>(std::make_shared<var_t>(v), e.term, op_t::assign)}
        {
        }

        // Constructor for assignment operations with a specific operator
        expr_t(const var_t& v, const expr_t& e, op_t op)
            : term{std::make_shared<assign_t>(std::make_shared<var_t>(v), e.term, op)}
        {
        }

        double operator()(state_t& s) const
        {
            if (!term)
                throw std::logic_error{"missing term"};
            return term->operator()(s);
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

    inline expr_t operator+=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::add}; }
    inline expr_t operator-=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::sub}; }
    inline expr_t operator*=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::mult}; }
    inline expr_t operator/=(const var_t& v, const expr_t& e) { return expr_t{v, e, op_t::div}; }

    inline expr_t operator*(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::mult}; }
    inline expr_t operator/(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, op_t::div}; }

    struct Printer : Visitor
    {
        std::ostream& os;
        const symbol_table_t& symbols;

        Printer(std::ostream& os, const symbol_table_t& symbols)
            : os{os}, symbols{symbols} {}

        void visit(const const_t& c) override {
            os << c.value;
        }

        void visit(const var_t& v) override {
            os << symbols.names[v.id];
        }

        void visit(const unary_t& u) override {
            if (u.op == op_t::minus) {
                os << "-";
            } else if (u.op == op_t::plus) {
                os << "+";
            }
            u.operand->accept(*this);
        }

        void visit(const binary_t& b) override {
            os << "(";
            b.term1->accept(*this);
            switch (b.op) {
            case op_t::add: os << " + "; break;
            case op_t::sub: os << " - "; break;
            case op_t::mult: os << " * "; break;
            case op_t::div: os << " / "; break;
            default: break;
            }
            b.term2->accept(*this);
            os << ")";
        }

        void visit(const assign_t& a) override {
            a.var->accept(*this);
            switch (a.op) {
            case op_t::assign: os << " <<= "; break;
            case op_t::add: os << " += "; break;
            case op_t::sub: os << " -= "; break;
            case op_t::mult: os << " *= "; break;
            case op_t::div: os << " /= "; break;
            default: break;
            }
            a.term->accept(*this);
        }

        void visit(const expr_t& e) override {
            e.term->accept(*this);
        }
    };

    struct PrintTerm {
        const term_t& term;
        const symbol_table_t& symbols;
    };

    inline std::ostream& operator<<(std::ostream& os, const PrintTerm& pt) {
        Printer printer{os, pt.symbols};
        pt.term.accept(printer);
        return os;
    }


}



#endif // INCLUDE_ALGEBRA_HPP
