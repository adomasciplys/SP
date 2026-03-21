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

/** Helper to write container elements with comma separators (for arrays and object values) */
template <typename Range, typename Fn>
void write_items(json_ostream& j, const Range& range, Fn&& fn) {
    bool first = true;
    for (const auto& item : range) {
        if (!first) j.os << ',';
        first = false;
        fn(item);
    }
}

/** Helper to write JSON objects with field visitors (wraps fields in {} and manages json_writer) */
template <typename Fn>
void write_object(json_ostream& j, Fn&& fn) {
    j.os << '{';
    json_writer writer{.out=j};
    fn(writer);
    j.os << '}';
}

/** Recursively write tuple elements using visitor pattern (converts tuple indices to field names "0", "1", "2", etc.) */
template <std::size_t I = 0, typename Tuple>
requires(is_std_tuple_v<Tuple>)
void write_tuple(json_writer& writer, const Tuple& t) {
    if constexpr (I < std::tuple_size_v<Tuple>) {
        writer.visit(std::to_string(I), std::get<I>(t));
        write_tuple<I+1>(writer, t);
    }
}

/** Main JSON serialization operator. Dispatches based on type using compile-time checks (if constexpr) */
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
        write_items(j, v, [&](const auto& kv) {
            j << kv.first;
            j.os << ':';
            j << kv.second;
        });
        j.os << '}';
    }
    else if constexpr (is_container_v<T>) {
        j.os << '[';
        write_items(j, v, [&](const auto& elem) { j << elem; });
        j.os << ']';
    }
    else if constexpr (accepts_v<T, json_writer>) {
        write_object(j, [&](json_writer& w) { v.accept(w); });
    }
    else if constexpr (is_std_tuple_v<T>) {
        write_object(j, [&](json_writer& w) { write_tuple(w, v); });
    }

    return j;
}

template <typename T>
json_ostream& operator<<(json_ostream&& j, const T& value)
{
    return j << value;
}

#endif  // STATIC_VISITOR_JSON_OUTPUT_HPP
