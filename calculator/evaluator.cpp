#include "evaluator.hpp"

#include "const.hpp"
#include "unary.hpp"
#include "var.hpp"
#include "binary.hpp"
#include "assign.hpp"
#include "expr.hpp"

namespace calculator
{
    Evaluator::Evaluator(double result, state_t& state) : _result(result), _state(state) {}

    double Evaluator::result() const
    {
        return _result;
    }

    void Evaluator::visit(const const_t& c)
    {
        _result = c.value();
    }

    void Evaluator::visit(const var_t& v)
    {
        _result = _state.at(v.id());
    }

    void Evaluator::visit(const unary_t& u)
    {
        if (! u.operand())
            throw std::logic_error{"missing operand for unary operation"};

        // Select based on operator
        switch (u.op())
        {
        case plus:
            u.operand()->accept(*this);
            break;
        case minus:
            u.operand()->accept(*this);
            _result = -_result;
            break;
        default:
            throw std::logic_error{"unsupported unary operation"};
        }
    }

    void Evaluator::visit(const binary_t& b)
    {
        if (!b.term1())
            throw std::logic_error{"missing term 1"};
        if (!b.term2())
            throw std::logic_error{"missing term 2"};

        b.term1()->accept(*this); // Evaluate first term
        double val1 = _result;
        b.term2()->accept(*this); // Evaluate second term
        double val2 = _result;

        switch (b.op())
        {
        case add:
            _result = val1 + val2;
            break;
        case sub:
            _result = val1 - val2;
            break;
        case mult:
            _result = val1 * val2;
            break;
        case div:
            _result = safe_divide(val1, val2);
            break;
        default:
            throw std::logic_error{"unsupported binary operation"};
        }
    }

    void Evaluator::visit(const assign_t& a)
    {
        if (!a.var())
            throw std::logic_error{"missing variable"};
        if (!a.term())
            throw std::logic_error{"missing term"};

        a.term()->accept(*this); // Evaluate rhs
        const double rhs = _result;

        // Get a reference to var in state where we want to store the new value
        double& lhs = _state[a.var()->id()];

        switch (a.op())
        {
        case assign:
            lhs = rhs;
            break;
        case add:
            lhs = lhs + rhs; // +=
            break;
        case sub:
            lhs = lhs - rhs; // -=
            break;
        case mult:
            lhs = lhs * rhs; // *=
            break;
        case div:
            lhs = safe_divide(lhs, rhs); // /=
            break;
        default:
            throw std::logic_error{"unsupported assignment operation"};
        }

        // Assignment expressions evaluate to the assigned/updated variable value.
        _result = lhs;
    }

    void Evaluator::visit(const expr_t& e)
    {
        if (e.term)
            e.term->accept(*this);
    }

}
