#ifndef STATIC_VISITOR_META_HPP
#define STATIC_VISITOR_META_HPP
#include <type_traits>
#include <concepts>
#include <string>

template <typename T>
// remove_cref_t gives the underlying type removing const, volatile and references
// So, we remove the "wrapper" and then check if it is equal to bool, in all other cases this predicate is false
concept is_bool_v = std::same_as<std::remove_cvref_t<T>, bool>;

template <typename T>
concept is_number_v = (
    std::integral<std::remove_cvref_t<T>> || // is integer check
    std::floating_point<std::remove_cvref_t<T>>) && // is floating point check
    !std::same_as<std::remove_cvref_t<T>, bool>; // not a bool check

template <typename T>
concept is_character_v = std::same_as<std::remove_cvref_t<T>, char>;

template <typename T>
// Check if we can convert the type to std::string_view, very smart
concept is_string_v = std::is_convertible_v<std::remove_cvref_t<T>, std::string_view>;

template <typename T>
// Inspired by slides
concept is_container_v =
 requires(std::remove_reference_t<T>& c) {
    std::begin(c);
    std::end(c);
 } && !is_string_v<T>;

template <typename Data, typename Visitor>
concept accepts_v =
 requires(std::remove_reference_t<Data>& data,
 std::remove_reference_t<Visitor>& visitor) {
    data.accept(visitor);
 };

template <typename T>
// Associative containers (map, unordered_map, etc.) have a mapped_type member
// This distinguishes them from other containers like vector or set
concept is_associative_container_v =
 requires {
    typename std::remove_cvref_t<T>::mapped_type;
 } && is_container_v<T>; // Use our previously declared is_container

template <typename T, typename ElementType>
concept is_container_of_v =
 is_container_v<T> &&
 std::same_as<typename std::remove_cvref_t<T>::value_type, ElementType>;

template <typename T>
concept is_container_of_int_v = is_container_of_v<T, int>;



#endif  // STATIC_VISITOR_META_HPP
