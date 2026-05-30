#include "stack.hpp"
#include <vector>
#include <sstream>

namespace stp {

void Stack::grow()
{
    std::size_t newCap = (_capacity == 0) ? 4 : _capacity * 2;

    double* newData = new double[newCap];  // allocate new array
    for (std::size_t i = 0; i < _size; ++i) {
        newData[i] = _data[i];  // move elements over
    }
    delete[] _data;  // free old array
    _data = newData;
    _capacity = newCap;
}

Stack::Stack(std::initializer_list<double> init)
{
    for (double value : init) {
        push(value);
    }
}

Stack::~Stack()
{
    delete[] _data;  // free allocated memory
}

bool Stack::is_empty() const
{
    return _size == 0;
}

std::size_t Stack::size() const
{
    return _size;
}

void stp::Stack::push(double value)
{
    if (_size == _capacity) {
        grow();
    }
    _data[_size++] = value;
}

double& Stack::top()
{
    assert(_size != 0);
    return _data[_size - 1];
}

const double& Stack::top() const
{
    assert(_size != 0);
    return _data[_size - 1];
}

double Stack::pop()
{
    assert(_size != 0);
    return _data[--_size];
}

double Stack::get(std::size_t index) const
{
    assert(index <= _size - 1);
    return _data[index];
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