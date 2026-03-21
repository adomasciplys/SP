#ifndef STATIC_VISITOR_JSON_INPUT_HPP
#define STATIC_VISITOR_JSON_INPUT_HPP

#include "meta.hpp"

#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

/** TODO: implement json_istream adapter with json input operations
 * The goal is to exercise meta-programming and not have complete JSON (Unicode support is beyond the scope).
 * Parsing should follow the type structure rather than the content of the input stream.
 * Visitor parsing may depend on the order of fields, which is OK for this exercise.
 */

struct json_istream
{
    std::istream& is;
};

/** Check if the next character is a closing bracket (empty array) */
inline bool is_empty_array(json_istream& j) {
    if (j.is.peek() == ']') {
        char bracket;
        j.is >> bracket;  // Consume ']'
        return true;
    }
    return false;
}

/** Read array elements until closing bracket */
template <typename Container>
requires is_container_v<Container>
void read_container_elements(json_istream& j, Container& v) {
    while (true) {
        typename Container::value_type elem;
        j >> elem;  // Recursively read element
        v.insert(v.end(), elem);  // Works for vectors, sets, etc.
        j.is >> std::ws;

        char ch;
        j.is >> ch; // Either "," (continue) or ']' (stop)
        if (ch == ']') {
            break;  // End of container
        }
    }
}

/** Visitor pattern support for reading JSON */
struct json_reader
{
    json_istream& in;

    template <typename Data>

    void visit(std::string_view name, Data& value)
    {
        // Skip whitespace
        in.is >> std::ws;

        // Read field name (with quotes)
        char quote;
        in.is >> quote;  // Opening quote
        std::string field_name;
        std::getline(in.is, field_name, '"');  // Read until closing quote

        // Skip whitespace and colon
        in.is >> std::ws;
        char colon;
        in.is >> colon;

        // Read the value recursively
        in >> value;

        // Skip whitespace and comma (if present)
        in.is >> std::ws;
        char ch;
        if (in.is.peek() == ',') {
            in.is >> ch;
        }
    }
};

template <typename T>
json_istream& operator>>(json_istream& j, T& v)
{
    if constexpr (is_bool_v<T>) {
        std::string bool_str;
        j.is >> bool_str;
        v = (bool_str == "true");
    }
    if constexpr (is_number_v<T>) {
        j.is >> v; // Handled natively bi istream
    }
    if constexpr (is_string_v<T>) {
        j.is >> std::quoted(v);
    }
    if constexpr (is_container_v<T>) {
        v.clear(); // Container might already contain values

        // Read opening bracket
        j.is >> std::ws;
        char bracket;
        j.is >> bracket;  // Consume '['
        j.is >> std::ws;

        // Check if empty array
        if (is_empty_array(j)) {
            return j;
        }

        // Read elements
        read_container_elements(j, v);
    }
    

    return j;
}

/** Helper for rvalue reference */
template <typename T>
json_istream& operator>>(json_istream&& j, T& value)
{
    return j >> value;
}

#endif  // STATIC_VISITOR_JSON_INPUT_HPP
