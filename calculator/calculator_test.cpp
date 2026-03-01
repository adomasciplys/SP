#include "calculator.hpp"

#include <doctest/doctest.h>

#include <sstream>

TEST_CASE("Calculate expressions lazily")
{
    auto sys = calculator::symbol_table_t{}; // create a symbol table for variables
    auto a = sys.var("a", 2); // create a variable with name "a" and initial value of 2
    auto b = sys.var("b", 3); // create a variable with name "b" and initial value of 3
    auto c = sys.var("c"); // create a variable with name "c" and default-initialize with 0
    auto state = sys.state(); // create a system state initialized with variable initial values
    auto os = std::ostringstream(); // string stream to output to

    SUBCASE("Reading the value of a variable from state")
    {
        CHECK(a(state) == 2);
        CHECK(b(state) == 3);
        CHECK(c(state) == 0);
    }
    SUBCASE("Unary operations")
    {
        CHECK((+a)(state) == 2);
        CHECK((-b)(state) == -3);
        CHECK((-c)(state) == 0);
    }
    SUBCASE("Addition and subtraction")
    {
        CHECK((a + b)(state) == 5);
        CHECK((a - b)(state) == -1);
        // the state should not have changed:
        CHECK(a(state) == 2);
        CHECK(b(state) == 3);
        CHECK(c(state) == 0);
    }
    SUBCASE("Assignment expression evaluation")
    {
        CHECK(c(state) == 0);
        CHECK((c <<= b - a)(state) == 1);
        CHECK(c(state) == 1);


        CHECK((c += b - a * c)(state) == 2);
        CHECK(c(state) == 2);
        CHECK((c += b - a * c)(state) == 1);
        CHECK(c(state) == 1);

        CHECK((c -= a)(state) == -1);
        CHECK(c(state) == -1);

        CHECK((c *= b)(state) == -3);
        CHECK(c(state) == -3);

        CHECK((c /= b)(state) == -1);
        CHECK(c(state) == -1);


        CHECK_THROWS_MESSAGE((c - a += b - c), "assignment destination must be a variable expression");
    }
    SUBCASE("Parenthesis")
    {
        CHECK((a - (b - c))(state) == -1);
        CHECK((a - (b - a))(state) == 1);
    }

    // TODO: implement multiplication and division
    SUBCASE("Evaluation of multiplication and division")
    {
        CHECK((a * b)(state) == 6);
        CHECK((a / b)(state) == 2. / 3);
        CHECK_THROWS_MESSAGE((a / c)(state), "division by zero");
    }
    SUBCASE("Mixed addition and multiplication")
    {
        CHECK((a + a * b)(state) == 8);
        CHECK((a - b / a)(state) == 0.5);
    }

    SUBCASE("Constant expressions")
    {
        CHECK((7 + a)(state) == 9);
        CHECK((a - 7)(state) == -5);
    }

    SUBCASE("Store expression and evaluate lazily")
    {
        auto expr = (a + b) * c;
        auto c_4 = c <<= 4;
        CHECK(expr(state) == 0);
        CHECK(c_4(state) == 4);
        CHECK(expr(state) == 20);
    }

    SUBCASE("Printing expressions with visitor pattern")
    {
        auto expr1 = a + b;
        os << calculator::PrintTerm{*expr1.term, sys};
        CHECK(os.str() == "(a + b)");

        os.str("");
        auto expr2 = -c;
        os << calculator::PrintTerm{*expr2.term, sys};
        CHECK(os.str() == "-c");

        os.str("");
        auto expr3 = a * b - c;
        os << calculator::PrintTerm{*expr3.term, sys};
        CHECK(os.str() == "((a * b) - c)");

        os.str("");
        auto expr4 = c <<= a + b;
        os << calculator::PrintTerm{*expr4.term, sys};
        CHECK(os.str() == "c <<= (a + b)");

        os.str("");
        auto expr5 = 7 + a;
        os << calculator::PrintTerm{*expr5.term, sys};
        CHECK(os.str() == "(7 + a)");
    }

SUBCASE("Evaluator visitor pattern")
{
    auto evaluator = calculator::Evaluator{0.0, state};

    // Test constant evaluation
    auto expr1 = calculator::expr_t{7};
    expr1.term->accept(evaluator);
    CHECK(evaluator.result() == 7);

    // Test variable evaluation
    a.accept(evaluator);
    CHECK(evaluator.result() == 2);

    // Test binary expression
    auto expr2 = a + b;
    expr2.term->accept(evaluator);
    CHECK(evaluator.result() == 5);

    // Test unary expression
    auto expr3 = -b;
    expr3.term->accept(evaluator);
    CHECK(evaluator.result() == -3);

    // Test assignment
    auto expr4 = c <<= a + b;
    expr4.term->accept(evaluator);
    CHECK(evaluator.result() == 5);
}


}
