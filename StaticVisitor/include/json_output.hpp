#ifndef STATIC_VISITOR_JSON_OUTPUT_HPP
#define STATIC_VISITOR_JSON_OUTPUT_HPP

#include "meta.hpp"
#include <iomanip>
#include <iostream>
#include <string_view>
#include <tuple>

struct json_ostream
{
    std::ostream& os;
};

/** Visitor pattern support for writing JSON */
struct json_writer
{
    json_ostream& out;
    bool first_field = true;

    template <typename Data>
    void visit(std::string_view name, const Data& value)
    {
        if (!first_field) {
            out.os << ',';
        }
        first_field = false;

        out.os << '\"' << name << "\":";
        out << value;
    };
};

template <std::size_t I = 0, typename Tuple>
requires(is_std_tuple_v<Tuple>)
void write_tuple(json_writer& writer, const Tuple& t) {
    if constexpr (I < std::tuple_size_v<Tuple>) {
        writer.visit(std::to_string(I), std::get<I>(t));
        write_tuple<I+1>(writer, t);
    }
}

template <typename T>
json_ostream& operator<<(json_ostream& j, const T& v)
{
    if constexpr (is_bool_v<T>) {
        j.os << (v ? "true" : "false");
    }
    else if constexpr (is_number_v<T>) {
        j.os << v;
    }
    else if constexpr (is_string_v<T>) {
        j.os << std::quoted(v);
    }
    else if constexpr (is_associative_container_v<T>) {
        j.os << '{';
        bool first = true;
        for (const auto& [key, value] : v) {
            if (!first) {
                j.os << ',';
            }
            first = false;
            j << key;  // Output the key
            j.os << ':';
            j << value;  // Output the value
        }
        j.os << '}';
    }
    else if constexpr (is_container_v<T>) {
        j.os << '[';
        bool first = true;
        for (const auto& elem : v) {
            if (!first) {
                j.os << ',';
            }
            first = false;
            j << elem;  // Recursive call
        }
        j.os << ']';
    }
    else if constexpr (accepts_v<T, json_writer>) {
        j.os << '{';
        json_writer writer{.out=j};
        v.accept(writer);
        j.os << '}';
    }
    else if constexpr (is_std_tuple_v<T>) {
        j.os << '{';
        json_writer writer{.out=j};
        write_tuple(writer, v);
        j.os << '}';
    }

    return j;
}

template <typename T>
json_ostream& operator<<(json_ostream&& j, const T& value)
{
    return j << value;
}

#endif  // STATIC_VISITOR_JSON_OUTPUT_HPP
