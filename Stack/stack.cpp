#include "stack.hpp"
#include <vector>
#include <sstream>

/// TODO: write the definitions (implementation) of the stack library here:
/// TODO: implement Stack member functions within stp namespace
/// TODO: implement non-member functions
namespace stp {

void Stack::grow()
{
    std::size_t newCap = (capacity_ == 0) ? 4 : capacity_ * 2;

    double* newData = new double[newCap];  // allocate new array
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = std::move(data_[i]);  // move elements over
    }
    delete[] data_;  // free old array
    data_ = newData;
    capacity_ = newCap;
}

Stack::Stack(std::initializer_list<double> init)
{
    for (double value : init) {
        push(value);
    }
}

Stack::Stack() = default;  // default constructor

Stack::~Stack()
{
    delete[] data_;  // free allocated memory
}

bool Stack::is_empty() const
{
    return size_ == 0;
}

std::size_t Stack::size() const
{
    return size_;
}

void stp::Stack::push(double value)
{
    if (size_ == capacity_) {
        grow();
    }
    data_[size_++] = value;
}

double& Stack::top()
{
    assert(size_ != 0);
    return data_[size_ - 1];
}

const double& Stack::top() const
{
    assert(size_ != 0);
    return data_[size_ - 1];
}

double Stack::pop()
{
    assert(size_ != 0);
    return data_[--size_];
}

double Stack::get(std::size_t index) const
{
    assert(index <= size_ - 1);
    return data_[index];
}

std::ostream& operator<<(std::ostream& os, const Stack& s)
{
    os << "Stack[";
    if (!s.is_empty()) {
        for (std::size_t i = s.size(); i-- > 0;) {
            if (i != s.size() - 1) os << ",";
            os << s.get(i);
        }
    }
    return os << "]";
}

}  // namespace stp

namespace std {

std::string to_string(const stp::Stack& stack)
{
    std::ostringstream os{};
    os << stack;
    return os.str();
}

}  // namespace std