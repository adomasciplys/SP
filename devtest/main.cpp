#include <iostream> // cout
#include <sstream>  // stringstream
#include <iomanip>  // setw, setfill
#include <future>   // async
#include <string_view>
#include <algorithm> // sort
#include <vector>
#include <random> // random_device, mt19937, uniform_int_distribution
#include <execution> // execution::par

using dataset = std::vector<std::string>;
using namespace std::literals::string_literals;

std::string make_id(int32_t id) {
	auto os = std::ostringstream{};
	os << std::setw(3) << std::setfill('0') << id;
	return os.str();
}

dataset gen_data(size_t sz) {
	static auto dist = std::uniform_int_distribution<int32_t>{0,999};
	auto gen = std::mt19937{std::random_device{}()};
	auto res = dataset{};
	res.reserve(sz);
	for (auto i=0u; i<sz; ++i)
		if (i%2)
			res.push_back("id"s + make_id(dist(gen)));
		else
			res.push_back("ID"s + make_id(dist(gen)));
	return res;
}

void memory_abuse() {
    auto leak = new const char []{"hello"};
    std::cout << leak[100] << '\n';
}

int main() {
	constexpr auto N = 100;
	const auto cores = std::thread::hardware_concurrency();
	auto res = std::vector<std::future<dataset>>{};
	for (auto core = 0u; core < cores; ++core)
		res.emplace_back(std::async(gen_data, N / cores)); // part per core
	res.emplace_back(std::async(gen_data, N % cores)); // remainder
	auto data = dataset{};
	for (auto&& d: res) {
		auto part = d.get();
		data.insert(data.end(), part.begin(), part.end());
	}
#if __clang__ && __clang_major__ < 14
    // Clang<14 STL does not support parallel execution of algorithms
    std::sort(data.begin(), data.end(),
              [](const std::string& s1, const std::string& s2){
                  auto sv1 = std::string_view{s1}.substr(2);
                  auto sv2 = std::string_view{s2}.substr(2);
                  return sv1 < sv2;
              });
#else
    // Clang>=14, GCC>=11 and MSVC>=17 do support parallel execution
    std::sort(std::execution::par, data.begin(), data.end(),
		[](const std::string& s1, const std::string& s2){
			auto sv1 = std::string_view{s1}.substr(2);
			auto sv2 = std::string_view{s2}.substr(2);
			return sv1 < sv2;
	});
#endif
	auto cnt = 0u;
	for (const auto& d: data) {
		std::cout << d << ',';
		if (++cnt == 10) {
			std::cout << '\n';
			cnt = 0;
		}
	}
    std::cout.flush();
    memory_abuse();
}
