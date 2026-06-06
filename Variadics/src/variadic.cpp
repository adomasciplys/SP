#include <functional>  // std::ref
#include <tuple>
#include <type_traits>
#include <sstream>
#include <iomanip>	// quoted

#include <doctest/doctest.h>

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
std::string concat(Args&&...)
{
	/** TODO: implement string concatenation of arguments using ostringstream */
	auto os = std::ostringstream{};
	return os.str();
}

TEST_CASE("Variadic argument concatenation")
{
	CHECK(concat(3, " plus ", 4, " equals ", 7) == "3 plus 4 equals 7");
	CHECK(concat("3.14", " equals ", 3.14) == "3.14 equals 3.14");
}

/** Log arguments in JSON format */
template <typename... Args>
std::string json_log(Args...)
{
	/** TODO: implement concatenation in JSON format:
	 * '[' arg0 ',' arg1 ',' ... ']'
	 * where string arguments are quoted (using std::quoted)
	 * DRY code by using helper function to stringify based on type
	 */
	auto os = std::ostringstream{};
	return os.str();
}

/** Log std::tuple in JSON format */
template <typename... Args>
std::string json_log(const std::tuple<Args...>&)
{
	/** TODO: implement concatenation in JSON format:
	 * '{' arg0 ',' arg1 ',' ... '}'
	 * where string arguments are quoted (using std::quoted)
	 * try outputting the first and then tag-dispatch the rest with make_index_sequence
	 */
	auto os = std::ostringstream{};
	return os.str();
}

template <typename C>
std::string json_log(const C&)
{
	/** TODO: implement concatenation of a container content in JSON format:
	 * '[' c[0] ',' c[1] ',' ... ']'
	 * where string arguments are quoted (using std::quoted)
	 * use SFINAE to restrict the overloading to containers which do not contain characters
	 */
	auto os = std::ostringstream{};
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
