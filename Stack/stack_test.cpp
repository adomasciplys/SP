/// Tests for stp::Stack
/// Author: Marius Mikucionis <marius@cs.aau.dk>

/// Keep debug (assertions) even in Release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "stack.hpp"

#include <type_traits>
#include <sstream>
#include <iostream>

#include <cassert>

using stp::Stack;

/// Test Stack is_empty method
void test_stack_empty()
{
    const Stack stack{};       // immutable instance
    assert(stack.is_empty());  // initially the stack is empty
}

/// Test Stack push method
void test_stack_push()
{
    Stack stack{};
    assert(stack.is_empty());   // initially the stack is empty
    stack.push(1);              // push some element
    assert(!stack.is_empty());  // and stack must be non-empty
}

/// Test Stack top method
void test_stack_top()
{
    Stack stack{};
    stack.push(1);
    assert(stack.top() == 1);
    stack.top() = 2;  // modify the top of the stack directly
    assert(stack.top() == 2);
    {
        const Stack& stack2 = stack;  // const reference, immutable instance
        assert(stack2.top() == 2);    // allow checking the top
        // modifying the top of the immutable stack is not allowed:
        assert(std::is_const_v<std::remove_reference_t<decltype(stack2.top())>>);
    }
}

/// Test Stack pop method
void test_stack_pop()
{
    Stack stack{};
    assert(stack.is_empty());
    stack.push(1);
    assert(!stack.is_empty());
    assert(stack.top() == 1);
    stack.push(2);
    assert(!stack.is_empty());
    assert(stack.top() == 2);
    stack.push(3);
    assert(!stack.is_empty());
    assert(stack.top() == 3);
    assert(stack.pop() == 3);
    assert(!stack.is_empty());
    assert(stack.top() == 2);
    assert(stack.pop() == 2);
    assert(!stack.is_empty());
    assert(stack.top() == 1);
    assert(stack.pop() == 1);
    assert(stack.is_empty());
}

/// Test Stack stream output operator
void test_stack_print()
{
    Stack stack{};
    {
        std::ostringstream ss{};
        ss << stack;
        assert(ss.str() == "Stack[]");
    }
    stack.push(1);
    {
        std::ostringstream ss{};
        ss << stack;
        assert(ss.str() == "Stack[1]");
    }
    stack.push(2);
    {
        std::ostringstream ss{};
        ss << stack;
        std::cerr << "got: '" << ss.str() << "'\n";
        assert(ss.str() == "Stack[2,1]");
    }
    stack.push(3);
    {
        std::ostringstream ss{};
        ss << stack;
        assert(ss.str() == "Stack[3,2,1]");
    }
}

/// Test Stack to string conversion
void test_stack_string()
{
    Stack stack{};
    stack.push(1);
    stack.push(2);
    stack.push(3);
    std::string str = std::to_string(stack);
    assert(str == "Stack[3,2,1]");
}

/// Test Stack destructor
void test_stack_destructor()
{
    Stack stack{};
    for (int i = 0; i < 1000000; ++i)
        stack.push(i);
}

/// Test Stack constructor
void test_stack_constructor()
{
    Stack stack{1, 2, 3};
    assert(!stack.is_empty());
    assert(stack.pop() == 3);
    assert(!stack.is_empty());
    assert(stack.pop() == 2);
    assert(!stack.is_empty());
    assert(stack.pop() == 1);
    assert(stack.is_empty());
}

int main()
{
    test_stack_empty();
    test_stack_push();
    test_stack_top();
    test_stack_pop();
    test_stack_print();
    test_stack_string();
    test_stack_destructor();
    test_stack_constructor();
    std::cout << "Tests passed" << std::endl;
    return 0;
}