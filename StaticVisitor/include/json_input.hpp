#ifndef STATIC_VISITOR_JSON_INPUT_HPP
#define STATIC_VISITOR_JSON_INPUT_HPP

#include "meta.hpp"
#include <iomanip>
#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>

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

template <typename Fn>
void read_object(json_istream& j, Fn&& fn) {
    char brace;
    j.is >> brace; // consume '{'
    json_reader reader{.in=j};
    fn(reader);
    j.is >> brace; // consume '}'
}

template <std::size_t I = 0, typename Tuple>
requires(is_std_tuple_v<Tuple>)
void read_tuple(json_reader& reader, Tuple& t) {
    if constexpr (I < std::tuple_size_v<Tuple>) {
        reader.visit("", std::get<I>(t));
        read_tuple<I+1>(reader, t);
    }
}

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
    else if constexpr (is_associative_container_v<T>) {
        v.clear();
        char ch;
        j.is >> ch;  // consume '{'
        j.is >> ch;  // first non-ws char: '}' if empty, otherwise start of first key
        if (ch != '}') {
            j.is.putback(ch);
            do {
                typename T::key_type key;
                typename T::mapped_type value;
                j >> key;      // Read the key
                j.is >> ch;    // consume ':'
                j >> value;    // Read the value
                v.insert({key, value});
                j.is >> ch;    // ',' or '}'
            } while (ch != '}');
        }
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
        read_object(j, [&](json_reader& r) { v.accept(r); });
    }
    else if constexpr (is_std_tuple_v<T>) {
        read_object(j, [&](json_reader& r) { read_tuple(r, v); });
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
