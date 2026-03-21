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
        if (in.is.peek() == ',') in.is.ignore();
    }
};

/** Helper to read comma-separated items until a closing delimiter (opener is consumed internally) */
template <typename Fn>
void read_items(json_istream& j, Fn&& fn) {
    char ch;
    j.is >> ch; // consume opening delimiter ('[' or '{')
    const char close = (ch == '[') ? ']' : '}';
    j.is >> ch; // first non-ws char: closer if empty, otherwise start of first element
    if (ch != close) {
        j.is.putback(ch);
        do {
            fn();
            j.is >> ch; // ',' or closer
        } while (ch != close);
    }
}

/** Helper to read JSON objects with field visitors (consumes {} braces and creates json_reader for field parsing) */
template <typename Fn>
void read_object(json_istream& j, Fn&& fn) {
    char brace;
    j.is >> brace; // consume '{'
    json_reader reader{.in=j};
    fn(reader);
    j.is >> brace; // consume '}'
}

/** Recursively parse tuple elements using visitor pattern (extracts each element by index from JSON array) */
template <std::size_t I = 0, typename Tuple>
requires(is_std_tuple_v<Tuple>)
void read_tuple(json_reader& reader, Tuple& t) {
    if constexpr (I < std::tuple_size_v<Tuple>) {
        reader.visit("", std::get<I>(t));
        read_tuple<I+1>(reader, t);
    }
}

/** Main JSON deserialization operator. Dispatches based on type using compile-time checks (if constexpr) */
template <typename T>
json_istream& operator>>(json_istream& j, T& v)
{
    if constexpr (is_bool_v<T>) {
        j.is >> std::boolalpha >> v; // boolalpha can read "true", "false" -> Written to v
    }
    else if constexpr (is_number_v<T>) {
        j.is >> v;
    }
    else if constexpr (is_string_v<T>) {
        j.is >> std::quoted(v);
    }
    else if constexpr (is_associative_container_v<T>) {
        v.clear();
        read_items(j, [&] {
            typename T::key_type key;
            typename T::mapped_type value;
            char colon;
            j >> key;
            j.is >> colon;
            j >> value;
            v.insert({key, value});
        });
    }
    else if constexpr (is_container_v<T>) {
        v.clear();
        read_items(j, [&] {
            typename T::value_type elem;
            j >> elem;
            v.insert(v.end(), elem);
        });
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
