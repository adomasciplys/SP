#include "abc_csv.hpp"

#include <string>
#include <ranges>
#include <concepts>
#include <algorithm>
#include <charconv>  // std::from_chars
#include <iostream>

/// Helper to read files line by line with line numbers
struct Line
{
    std::string text;
    size_t line_nr{0};
    friend std::istream& operator>>(std::istream& is, Line& l)
    {
        ++l.line_nr;
        return std::getline(is, l.text);
    }
};

/// Reads the CSV data from input stream and returns a container of ABC structs.
std::vector<ABC> read_abc(std::istream& is)
{
    // TODO: Implement using ranges library, so that the tests below pass
    // TODO: Strive for one-statement solution (it may need extra utilities that's not accounted here)
    return {};
}
