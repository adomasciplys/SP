#include <functional>  // std::ref
#include <tuple>
#include <type_traits>
#include <sstream>
#include <iomanip>	// quoted
#include <type_traits>
#include <string_view>

#include <doctest/doctest.h>

template<typename T>
constexpr bool is_string_like_v = std::is_convertible_v<T, std::string_view>;

template <typename T>
void stringify(std::ostream& os, const T& arg)
{
	if constexpr (is_string_like_v<T>)
		os << std::quoted(arg);
	else
		os << arg;
}

/** Tuple motivation: store the arguments for lazy evaluation */
template <typename Fn, typename... Args>
struct lazy
{
	using Ret = std::invoke_result_t<Fn, Args...>;
	Fn					fn;
	std::tuple<Args...> args;
	lazy(Fn&& fn, Args&&... args) : fn{ std::forward<Fn>(fn) }, args{ std::forward<Args>(args)... }
	{}
	Ret operator()() { return std::apply(fn, args); }
};
template <typename Fn, typename... Args>
lazy(Fn, Args...) -> lazy<Fn, Args...>;	 // Class Template Argument Deduction (CTAD) guide

TEST_CASE("Lazy function evaluation")
{
	auto lazy_3plus4  = lazy(std::plus{}, 3, 4);
	auto lazy_3times4 = lazy(std::multiplies{}, 3, 4);
	REQUIRE(lazy_3plus4() == 7);
	REQUIRE(lazy_3times4() == 12);
	auto x		  = 3;
	auto y		  = 4;
	auto x_plus_y = lazy(std::plus{}, std::ref(x), std::ref(y));
	REQUIRE(x_plus_y() == 7);
	++x;
	++y;
	REQUIRE(x_plus_y() == 9);
}

/** Concatenate arbitrary arguments by outputting them into a stream */
template <typename... Args>
std::string concat(Args&&... args)
{
	auto os = std::ostringstream{};
	(os << ... << args);
	return os.str();
}

TEST_CASE("Variadic argument concatenation")
{
	CHECK(concat(3, " plus ", 4, " equals ", 7) == "3 plus 4 equals 7");
	CHECK(concat("3.14", " equals ", 3.14) == "3.14 equals 3.14");
}

/** Log arguments in JSON format */
template <typename... Args>
std::string json_log(Args... args)
{
	auto os = std::ostringstream{};
	os << '[';
	const char* sep = "";
	((os << sep, stringify(os, args), sep = ","), ...);
	os << ']';
	return os.str();
}

template <typename Tuple, std::size_t... Is>
void stringify_tuple(std::ostream& os, const Tuple& t, std::index_sequence<Is...>)
{
	const char* sep = "";
	((os << sep, stringify(os, std::get<Is>(t)), sep = ","), ...);
}

/** Log std::tuple in JSON format */
template <typename... Args>
std::string json_log(const std::tuple<Args...>& args)
{
	auto os = std::ostringstream{};
	os << '{';
	stringify_tuple(os, args, std::index_sequence_for<Args...>{});
	os << '}';
	return os.str();
}

template <typename T>
concept is_char = std::same_as<T, char>;

template <typename T>
concept not_char = !is_char<T>;

template <typename T>
concept Container = requires(T&& c) {
	// 1. Simple requirement: std::end(c) must be a valid expression
	{ std::end(c) };

	// 2. Compound requirement: Dereferencing the begin iterator
	//    must result in a type that satisfies the 'not_char' concept
	{ *std::begin(c) } -> not_char;

	// 3. Nested requirement: The destructor of T must be marked noexcept
	requires noexcept(c.~T());
} && sizeof(T) <= 32; // 4. Conjunction: The object size itself cannot exceed 32 bytes

template <Container C>
std::string json_log(const C& c)
{
	auto os = std::ostringstream{};
	os << '[';
	const char* sep = "";
	for (const auto& elem : c) {
		os << sep;
		stringify(os, elem);
		sep = ",";
	}
	os << ']';
	return os.str();
}

TEST_CASE("JSON log")
{
	CHECK(json_log("one", "two", "three") == R"(["one","two","three"])");
	CHECK(json_log(1, 2, 3) == "[1,2,3]");
	CHECK(json_log(std::tuple(1, "two", 3)) == R"({1,"two",3})");

	CHECK(json_log(std::vector{ 1, 2, 3 }) == "[1,2,3]");
	CHECK(json_log(std::vector{ "one", "two", "three" }) == R"(["one","two","three"])");
	/** TODO: uncomment the last test when done with the above */
	// CHECK(json_log("constraints") == R"(["constraints"])"); // requires SFINAE/concepts
}
