#include <format>
#include <iostream>
#include <list>
#include <vector>

void print(std::vector<int> v)
{
    std::cout << "{";
    for (size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i];
        if (i < v.size() - 1)
        {
            std::cout << ",";
        }
    }
    std::cout << "}" << std::endl;
}

/*std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    os << "{";
    bool first = true;
    for (int num : v)
    {
        if (!first)
        {
            os << ",";
        }
        os << num;
        first = false;
    }
    os << "}";
    return os;
}*/

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "{";
    bool first = true;
    for (T num : v)
    {
        if (!first)
        {
            os << ",";
        }
        os << num;
        first = false;
    }
    os << "}";
    return os;
}

template <typename T>
struct TD; // forward declaration (purposely without definition!)


template <typename C, typename = C::value_type>
std::ostream& operator<<(std::ostream& os, const C& container)
{
    os << "{";
    bool first = true;
    for (const auto& elem : container)
    {
        if (!first)
        {
            os << ",";
        }
        os << elem;
        first = false;
    }
    os << "}";
    return os;
}


int main()
{
    print({1, 2, 3});
    std::cout << std::vector{1,2,3} << std::endl;
    std::cout << std::vector{1,2,3} << std::endl;
    std::cout << std::vector{2.7,3.14} << std::endl;
    std::cout << std::vector{std::vector{"hello", "template"}, std::vector{"world"}} << std::endl;
    std::cout << std::vector{std::vector{"hello", "template"}, std::vector{"world"}} << std::endl;
    // TD<decltype(print)> t; --> 'TD<void(std::vector<int>)> t'
    // TD<decltype(std::cout << std::vector{1,2,3})> t2; --> TD<std::basic_ostream<char>&> t2

    std::cout << std::list{1,2,3} << std::endl;
    std::cout << std::array{1, 2, 3, 4, 5} << std::endl;
    return 0;
}