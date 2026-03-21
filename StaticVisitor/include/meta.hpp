#ifndef STATIC_VISITOR_META_HPP
#define STATIC_VISITOR_META_HPP
#include <tuple>
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
// Check if we can convert the type to std::string_view
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


// My initial idea was just to check for std::tuple_size, but
// it is specialized for std::array, std::pair, and std::tuple, so it can't distinguish between them.

// Flow (Personal Notes):
// Input Type: const std::tuple<int, double>&
// Concept sees: is_std_tuple_v<const std::tuple<int, double>&>
// Concept removes wrappers: std::remove_cvref_t<const std::tuple<int, double>&> = std::tuple<int, double>
// Concept instantiates struct: is_std_tuple_impl<std::tuple<int, double>>
// Struct matches specialization: template<typename... Args> struct is_std_tuple_impl<std::tuple<Args...>>
// Specialization inherits std::true_type
// Concept extracts: ::value = true
template <typename T>
struct is_std_tuple_impl : std::false_type {}; // Default: All types or NOT tuples

template <typename... Args>
struct is_std_tuple_impl<std::tuple<Args...>> : std::true_type {};

template <typename T>
concept is_std_tuple_v = is_std_tuple_impl<std::remove_cvref_t<T>>::value;



#endif  // STATIC_VISITOR_META_HPP
