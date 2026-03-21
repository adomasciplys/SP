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

/** Visitor pattern support for reading JSON */
struct json_reader
{
    json_istream& in;

    template <typename Data>
    void visit(std::string_view name, Data& value)
    {
        std::string field_name;
        char colon;
        in.is >> std::quoted(field_name) >> colon;
        in >> value;
        char ch;
        if (in.is.peek() == ',') in.is >> ch;
    }
};

template <typename T>
json_istream& operator>>(json_istream& j, T& v)
{
    if constexpr (is_bool_v<T>) {
        j.is >> std::boolalpha >> v;
    }
    else if constexpr (is_number_v<T>) {
        j.is >> v;
    }
    else if constexpr (is_string_v<T>) {
        j.is >> std::quoted(v);
    }
    else if constexpr (is_container_v<T>) {
        v.clear();
        char ch;
        j.is >> ch; // consume '['
        j.is >> ch; // first non-ws char: ']' if empty, otherwise start of first element
        if (ch != ']') {
            j.is.putback(ch);
            do {
                typename T::value_type elem;
                j >> elem;
                v.insert(v.end(), elem);
                j.is >> ch; // ',' or ']'
            } while (ch != ']');
        }
    }
    else if constexpr (accepts_v<T, json_reader>) {
        char brace;
        j.is >> brace; // consume '{'
        json_reader reader {.in=j};
        v.accept(reader);
        j.is >> brace; // consume '}'
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
