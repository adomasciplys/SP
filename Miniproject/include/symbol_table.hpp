#ifndef MINIPROJECT_SYMBOL_TABLE_HPP
#define MINIPROJECT_SYMBOL_TABLE_HPP

#include <concepts>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace stochastic {

// Constrains keys to types our backing store (std::unordered_map) can use:
// they must be hashable and equality-comparable.
template <typename T>
concept Hashable = requires(const T& t) {
    { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>;
};

template <typename Key, typename Value>
concept SymbolTableKey = Hashable<Key> && std::equality_comparable<Key>;

// A generic name -> value table.
// Wraps std::unordered_map with explicit failure semantics:
//   - insert throws std::invalid_argument if the key already exists,
//   - lookup throws std::out_of_range  if the key is not present.
template <typename Key, typename Value>
    requires SymbolTableKey<Key, Value>
struct SymbolTable
{
    // Insert a new key/value pair. Throws if `key` is already in the table.
    void insert(Key key, Value value)
    {
        auto [_, inserted] = _table.emplace(std::move(key), std::move(value));
        if (!inserted)
            throw std::invalid_argument("symbol_table: key already exists");
    }

    // Look up a value by key. Throws if `key` is not in the table.
    [[nodiscard]] const Value& lookup(const Key& key) const
    {
        auto it = _table.find(key);
        if (it == _table.end())
            throw std::out_of_range("symbol_table: key not found");
        return it->second;
    }

    [[nodiscard]] Value& lookup(const Key& key)
    {
        auto it = _table.find(key);
        if (it == _table.end())
            throw std::out_of_range("symbol_table: key not found");
        return it->second;
    }

    [[nodiscard]] bool contains(const Key& key) const noexcept { return _table.count(key) > 0; }
    [[nodiscard]] std::size_t size() const noexcept { return _table.size(); }
    [[nodiscard]] bool empty() const noexcept { return _table.empty(); }

private:
    std::unordered_map<Key, Value> _table;
};

}  // namespace stochastic

#endif  // MINIPROJECT_SYMBOL_TABLE_HPP
