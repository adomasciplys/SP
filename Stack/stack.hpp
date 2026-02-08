#ifndef INCLUDE_STACK_HPP
#define INCLUDE_STACK_HPP

/// TODO: write declarations only for the stack library here:
/// TODO: declare stp namespace
/// TODO: declare Stack struct/class within stp namespace
/// TODO: declare Stack member functions within Stack struct/class
/// TODO: declare Stack data members within Stack struct/class

#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <string>
#include <cassert>

namespace stp {

struct Stack {
private:
    double* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    void grow();

public:
    Stack();
    Stack(std::initializer_list<double> init);
    ~Stack();
    bool is_empty() const;
    std::size_t size() const;
    void push(double value);
    double& top();
    const double& top() const;
    double pop();

    //Only used for printing
    double get(std::size_t index) const;
};

// Print "Stack[]", "Stack[1]", "Stack[2,1]", "Stack[3,2,1]"
std::ostream& operator<<(std::ostream& os, const Stack& s);

// string conversion used by tests
std::string to_string(const Stack& s);

}

namespace std {
std::string to_string(const stp::Stack& s);
}


#endif  // INCLUDE_STACK_HPP
