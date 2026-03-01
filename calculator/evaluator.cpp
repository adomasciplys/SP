#include "evaluator.hpp"

#include "const.hpp"
#include "unary.hpp"
#include "var.hpp"
#include "binary.hpp"
#include "assign.hpp"

namespace calculator
{
    Evaluator::Evaluator(double result, state_t state) : _result(result), _state(state) {}

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
        case minus:
            u.operand()->accept(*this);
            _result = -_result;
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

        b.accept(*this); // Evaluate first term
        double val1 = _result;
        b.accept(*this); // Evaluate second term
        double val2 = _result;

        switch (b.op())
        {
        case add:
            _result = val1 + val2;
        case sub:
            _result = val1 - val2;
        case mult:
            _result = val1 * val2;
        case div:
            _result = safe_divide(val1, val2);
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

        a.accept(*this); // Evaluate rhs
        const double rhs = _result;

        // Get a reference to var in state where we want to store the new value
        double& lhs = _state[a.var()->id()];

        switch (a.op())
        {
        case assign:
            lhs = rhs;
        case add:
            lhs = lhs + rhs; // +=
        case sub:
            lhs = lhs - rhs; // -=
        case mult:
            lhs = lhs * rhs; // *=
        case div:
            lhs = safe_divide(lhs, rhs); // /=
        default:
            throw std::logic_error{"unsupported assignment operation"};
        }
    }

}
