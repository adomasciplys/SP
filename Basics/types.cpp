/// Exercise for exploring C++ type system (self-contained executable)
/// Author: Marius Mikucionis marius@cs.aau.dk

/// Keep the assertions enabled even in Release builds:
#ifdef NDEBUG
#undef NDEBUG
#endif // NDEBUG

#include <string>
#include <vector>
#include <array>
#include <limits>
#include <iostream>
#include <print>

#include <cassert>
#include <cinttypes>

int square(int n) { return n * n; }

/// C enum:
enum Color { COLOR_RED, COLOR_GREEN, COLOR_BLUE};

/// C++ enum with scope/namespace and type:
enum class Color2 : short { RED, GREEN, BLUE};

struct Empty {};

struct Int { int i; };

struct IntPtr { int* pi; };

struct IntRef { int& ri; };

struct Int4 {
    int a[4];
};

struct IntArray4 {
    std::array<int, 4> a;
};

struct IntVector4 {
    std::vector<int> v = std::vector<int>(4);
};

struct IntChar {
    int i;
    char c;
};

struct CharInt {
    char c;
    int i;
};

struct IntFloat : Int { // inherits from Int, i.e. field `i`
    float f;
};

union UIntOrChar4 {
    std::uint32_t u;
    char c[4];
};

void type_size_test() {
    // TODO: Fill out the correct sizes in bytes so that assertions do not fail
    // Arithmetic type sizes:
    assert(sizeof(int) == 4);
    assert(sizeof(unsigned) == 4);
    assert(sizeof(short) == 2);
    assert(sizeof(long) == 8);
    assert(sizeof(char) == 1);
    assert(sizeof(bool) == 1);
    assert(sizeof(float) == 4);
    assert(sizeof(double) == 8);
    // Other fundamental types:
    // assert(sizeof(void) == 0);
    assert(sizeof(nullptr) == 8);

    // Compare pointers and references:
    assert(sizeof(int*) == 8);
    assert(sizeof(float*) == 8);
    assert(sizeof(double*) == 8);
    assert(sizeof(int&) == 4);
    assert(sizeof(float&) == 4);
    assert(sizeof(double&) == 8);
    assert(sizeof(&square) == 8); // pointer to a function
    // TODO: compare various text representations:
    const char* cstr = "Hello World!";
    const char arr[] = "Hello World!";
    std::string cppstr = "Hello World!";
    assert(sizeof("Hello World!") == 13);
    assert(sizeof(cstr) == 8);
    assert(sizeof(arr) == 13);
    assert(sizeof(cppstr) == 32);

    // Sizes of custom types:

    // enum types:
    assert(sizeof(Color) == 4);
    assert(sizeof(COLOR_RED) == 4);
    assert(sizeof(Color2) == 2);
    assert(sizeof(Color2::RED) == 2);
    // struct types:
    assert(sizeof(Empty) == 1);
    assert(sizeof(Int) == 4);
    assert(sizeof(IntPtr) == 8);
    assert(sizeof(IntRef) == 8);
    assert(sizeof(Int4) == 16);
    assert(sizeof(IntArray4) == 16);
    assert(sizeof(IntVector4) == 24);
    assert(sizeof(IntChar) == 8);
    assert(sizeof(CharInt) == 8);
    assert(sizeof(IntFloat) == 8);
    // union types:
    assert(sizeof(UIntOrChar4) == 4);
}

void arithmetic_limit_test() {
    // TODO: Fill out the correct limits so that assertions do not fail (can be skipped)
    // Arithmetic type limits (minimum and maximum values):
    assert(std::numeric_limits<bool>::lowest() == 0);
    assert(std::numeric_limits<bool>::max() == 1);
    assert(std::numeric_limits<char>::lowest() == -128);
    assert(std::numeric_limits<char>::max() == 127);
    assert(std::numeric_limits<int>::lowest() == -2147483648);
    assert(std::numeric_limits<int>::max() == 2147483647);
    assert(std::numeric_limits<unsigned>::lowest() == 0);
    assert(std::numeric_limits<unsigned>::max() == 4294967295);
    assert(std::numeric_limits<short>::lowest() == -32768);
    assert(std::numeric_limits<short>::max() == 32767);
    assert(std::numeric_limits<float>::lowest() == -3.40282347e+38F);
    assert(std::numeric_limits<float>::max() == 3.40282347e+38F);
    assert(std::numeric_limits<double>::lowest() == -1.7976931348623157e+308);
    assert(std::numeric_limits<double>::max() == 1.7976931348623157e+308);
    // What happens beyond maximum?
    unsigned u = std::numeric_limits<unsigned>::max();
    u = u + 1; // OK
    assert(u == 0);
    int i = std::numeric_limits<int>::max();
    i = i + 1; // undefined behavior
    assert(i == -2147483648);
    auto f = std::numeric_limits<float>::max();
    f = f + 1;
    assert(f == 3.40282347e+38F);
    f = f * 2;
    assert(f == std::numeric_limits<float>::infinity());
}

void arithmetic_conversion_test() {
    bool b = true;
    char c = 65;
    short s = 129;
    int i = 3;
    long l = 5;
    long long ll = 7;
    float f = 3.14f;
    double d = 2.1718;
    // TODO: observe how a type can be obtained from an expression and compared:
    static_assert(std::same_as<decltype(b), bool>);
    static_assert(std::same_as<decltype(c), char>);
    static_assert(std::same_as<decltype(i), int>);
    static_assert(std::same_as<decltype(s), short>);
    static_assert(std::same_as<decltype(l), long>);
    static_assert(std::same_as<decltype(ll), long long>);
    static_assert(std::same_as<decltype(f), float>);
    static_assert(std::same_as<decltype(d), double>);
    // Comparisons:
    static_assert(std::same_as<decltype(s == i), bool>);
    static_assert(std::same_as<decltype(b == i), bool>);
    static_assert(std::same_as<decltype(f == f), bool>);
    static_assert(std::same_as<decltype(b == f), bool>);
    // TODO: uncomment and fix the correct types so that the assertions do not fail:
    // Arithmetic conversions:
    static_assert(std::same_as<decltype(b+b), int>);
    static_assert(std::same_as<decltype(c+c), int>);
    static_assert(std::same_as<decltype(+c), int>);
    static_assert(std::same_as<decltype(s+s), int>);
    static_assert(std::same_as<decltype(i+i), int>);
    static_assert(std::same_as<decltype(l+l), long>);
    static_assert(std::same_as<decltype(ll+ll), long long>);
    static_assert(std::same_as<decltype(f+f), float>);
    static_assert(std::same_as<decltype(d+d), double>);
    // Arithmetic promotion:
    static_assert(std::same_as<decltype(b+s), int>);
    static_assert(std::same_as<decltype(b+i), int>);
    static_assert(std::same_as<decltype(b+l), long>);
    static_assert(std::same_as<decltype(b+ll), long long>);
    static_assert(std::same_as<decltype(b+f), float>);
    static_assert(std::same_as<decltype(b+d), double>);
    static_assert(std::same_as<decltype(f+d), double>);
    static_assert(std::same_as<decltype(f+d), double>);
}

void endianness_test() {
    // Architecture is either little-endian or big-endian (how bytes are ordered)
    // TODO: comment out the failing assertions and determine the endianness of your architecture
    UIntOrChar4 ic = {0x12'34'56'78};
    // little-endian (the least significant byte is at the lowest address):
    assert(ic.c[0] == 0x78);
    assert(ic.c[1] == 0x56);
    assert(ic.c[2] == 0x34);
    assert(ic.c[3] == 0x12);
    // big-endian (the most significant byte is at the lowest address):
    //assert(ic.c[0] == 0x12);
    //assert(ic.c[1] == 0x34);
    //assert(ic.c[2] == 0x56);
    //assert(ic.c[3] == 0x78);
    char c4[4] = { 0x12, 0x34, 0x56, 0x78 };
    unsigned u = std::bit_cast<unsigned>(c4); // reinterprets the memory as different type
    assert(u == 0x78563412); // true when little-endian
    //assert(u == 0x12345678); // true when big-endian
    // Floating point numbers have their own endianness (independent of integers):
    float f = std::bit_cast<float>(c4);
    assert(f == 0x1.ac6824p+113);
    unsigned u0 = std::bit_cast<unsigned>(0.f);
    assert(u0 == 0x0000'0000);
    unsigned u1 = std::bit_cast<unsigned>(1.f);
    assert(u1 == 0x3f80'0000);
    unsigned u2 = std::bit_cast<unsigned>(2.f);
    assert(u2 == 0x4000'0000);
    unsigned u4 = std::bit_cast<unsigned>(4.f);
    assert(u4 == 0x4080'0000);
}

int main() {
    type_size_test();
    arithmetic_limit_test();
    arithmetic_conversion_test();
    endianness_test();
}