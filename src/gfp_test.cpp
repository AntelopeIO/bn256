#include <constants.h>
#include <gfp.h>
#include <iostream>
#include <iosfwd>

void print_gfp(const std::string& title, const bn256::gfp& gfp) {
    std::cout << title << std::endl;
    for( const auto& n : gfp) {
        const auto index = &n - &gfp[0];
        std::ostringstream ss;
        ss << std::hex << n;
        std::cout << index << ":" << "0x" << ss.str() << std::endl;
    }
}

// Tests that negation works the same way on both assembly-optimized and C++
// implementation.
void test_gfp_neg() {
    constexpr bn256::gfp n = {0x0123456789abcdef, 0xfedcba9876543210, 0xdeadbeefdeadbeef, 0xfeebdaedfeebdaed};
    constexpr bn256::gfp w = {0xfedcba9876543211, 0x0123456789abcdef, 0x2152411021524110, 0x0114251201142512};
    bn256::gfp h{};
    std::cout << std::endl << "test_gfp_neg:" << std::endl;
    bn256::gfp_neg(h, n);
    if (h != w) {
        std::cout << "negation mismatch:" << std::endl;
        print_gfp(std::string("have"), h);
        print_gfp(std::string("want"), w);
    }
}

// Tests that addition works the same way on both assembly-optimized and C++
// implementation.
void test_gfp_add() {
    constexpr bn256::gfp a = {0x0123456789abcdef, 0xfedcba9876543210, 0xdeadbeefdeadbeef, 0xfeebdaedfeebdaed};
    constexpr bn256::gfp b = {0xfedcba9876543210, 0x0123456789abcdef, 0xfeebdaedfeebdaed, 0xdeadbeefdeadbeef};
    constexpr bn256::gfp w = {0xc3df73e9278302b8, 0x687e956e978e3572, 0x254954275c18417f, 0xad354b6afc67f9b4};
    bn256::gfp h{};
    std::cout << std::endl << "test_gfp_add:" << std::endl;
    bn256::gfp_add(h, a, b);
    if (h != w) {
        std::cout << "addition mismatch:" << std::endl;
        print_gfp(std::string("have"), h);
        print_gfp(std::string("want"), w);
    }
}

// Tests that subtraction works the same way on both assembly-optimized and C++
// implementation.
void test_gfp_sub() {
    constexpr bn256::gfp a = {0x0123456789abcdef, 0xfedcba9876543210, 0xdeadbeefdeadbeef, 0xfeebdaedfeebdaed};
    constexpr bn256::gfp b = {0xfedcba9876543210, 0x0123456789abcdef, 0xfeebdaedfeebdaed, 0xdeadbeefdeadbeef};
    constexpr bn256::gfp w = {0x02468acf13579bdf, 0xfdb97530eca86420, 0xdfc1e401dfc1e402, 0x203e1bfe203e1bfd};
    bn256::gfp h{};
    std::cout << std::endl << "test_gfp_sub:" << std::endl;
    bn256::gfp_sub(h, a, b);
    if (h != w) {
        std::cout << "subtraction mismatch:" << std::endl;
        print_gfp(std::string("have"), h);
        print_gfp(std::string("want"), w);
    }
}

// Tests that multiplication works the same way on both assembly-optimized and C++
// implementation.
void test_gfp_mul() {
    constexpr bn256::gfp a = {0x0123456789abcdef, 0xfedcba9876543210, 0xdeadbeefdeadbeef, 0xfeebdaedfeebdaed};
    constexpr bn256::gfp b = {0xfedcba9876543210, 0x0123456789abcdef, 0xfeebdaedfeebdaed, 0xdeadbeefdeadbeef};
    constexpr bn256::gfp w = {0xcbcbd377f7ad22d3, 0x3b89ba5d849379bf, 0x87b61627bd38b6d2, 0xc44052a2a0e654b2};
    bn256::gfp h{};
    std::cout << std::endl << "test_gfp_mul:" << std::endl;
    bn256::gfp_mul(h, a, b);
    if (h != w) {
        std::cout << "multiplication mismatch:" << std::endl;
        print_gfp(std::string("have"), h);
        print_gfp(std::string("want"), w);
    }
}

int main() {
    test_gfp_neg();
    test_gfp_add();
    test_gfp_sub();
    test_gfp_mul();
    return 0;
}
