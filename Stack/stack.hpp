#ifndef INCLUDE_STACK_HPP
#define INCLUDE_STACK_HPP

#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <string>
#include <cassert>

namespace stp {

struct Stack
{
private:
    double* _data = nullptr; // pointer to our data
    std::size_t _size = 0; // initially we store no data
    std::size_t _capacity = 0; // how much space do we have left
    void grow(); // internal function used to expand the stack

public:
    Stack() = default; // constructor
    Stack(std::initializer_list<double> init); // construct with initializer list
    ~Stack(); // destructor

    [[nodiscard]] bool is_empty() const; // check if stack contains no elements
    [[nodiscard]] std::size_t size() const; // how many items are stored in the stack

    void push(double value); // add a new element to the stack
    double& top(); // get a reference to the top item on the stack (possible to modify)
    [[nodiscard]] const double& top() const; // get a const reference to top item on the stack (not possible to modify)
    double pop(); // pop an item from the stack and return the value to the caller

    // Only used for printing
    [[nodiscard]] double get(std::size_t index) const;
};

// Print "Stack[]", "Stack[1]", "Stack[2,1]", "Stack[3,2,1]"
std::ostream& operator<<(std::ostream& os, const Stack& s);

// string conversion used by tests
std::string to_string(const Stack& s);

}  // namespace stp

namespace std {
std::string to_string(const stp::Stack& s);
}

#endif  // INCLUDE_STACK_HPP
