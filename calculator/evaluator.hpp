#pragma once

#include "visitor.hpp"
#include <memory>
#include <vector>
#include "term.hpp"

namespace calculator
{
    struct const_t;
    struct unary_t;
    struct binary_t;
    struct assign_t;
    struct expr_t;
    class var_t;

    struct Evaluator : Visitor
    {
        Evaluator(double result, state_t& state);
        void visit(const const_t& c) override;
        void visit(const var_t& v) override;
        void visit(const unary_t& u) override;
        void visit(const binary_t& b) override;
        void visit(const assign_t& a) override;
        void visit(const expr_t& e) override;

        double result() const;

        private:
            double _result;
            state_t& _state;
    };
}

