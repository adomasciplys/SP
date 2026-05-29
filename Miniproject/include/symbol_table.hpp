#ifndef MINIPROJECT_SYMBOL_TABLE_HPP
#define MINIPROJECT_SYMBOL_TABLE_HPP

#include <concepts>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace stochastic {

// Constrains keys to types std::unordered_map can use: hashable and equality-comparable.
// https://en.cppreference.com/cpp/container/unordered_map
template <typename T>
concept Hashable = requires(const T& t) {
    { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>;
};

template <typename Key>
concept SymbolTableKey = Hashable<Key> && std::equality_comparable<Key>;

// A generic name -> value table.
// Wraps std::unordered_map with explicit errors:
//   - insert throws std::invalid_argument if the key already exists,
//   - lookup throws std::out_of_range if the key is not present (via map::at).
template <typename Key, typename Value>
    requires SymbolTableKey<Key>
struct SymbolTable
{
    // Insert a new key/value pair. Throws if key is already in the table.
    void insert(Key key, Value value)
    {
        // emplace returns std::pair<iterator, bool>
        auto [_, inserted] = _table.emplace(std::move(key), std::move(value));
        if (!inserted)
            throw std::invalid_argument("symbol_table: key already exists");
    }

    // unordered_map::at throws std::out_of_range if missing
    [[nodiscard]] const Value& lookup(const Key& key) const { return _table.at(key); }
    [[nodiscard]] Value& lookup(const Key& key){ return _table.at(key); }

    [[nodiscard]] bool contains(const Key& key) const noexcept { return _table.count(key) > 0; }
    [[nodiscard]] std::size_t size() const noexcept { return _table.size(); }
    [[nodiscard]] bool empty() const noexcept { return _table.empty(); }

private:
    std::unordered_map<Key, Value> _table;
};

}  // namespace stochastic

#endif  // MINIPROJECT_SYMBOL_TABLE_HPP
