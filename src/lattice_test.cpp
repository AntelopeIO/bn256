#include <lattice.h>
#include <cstdlib>
#include <random>
#include <iostream>
#include <iosfwd>
#include <constants.h>

using boost::multiprecision::int256_t;

void test_lattice_reduce_curve() {
    constexpr std::size_t max_bitlen = 130;
    // std::srand(bn256::order);
    auto ks = bn256::curve_lattice.decompose(3);
    bool too_large = false;
    bool not_positive = false;
    for (std::size_t i = 0; i < 2; i++) {
        auto bit_len = bn256::bitlen(ks[i]);
        std::cout << __FILE__ << ":"
                  << __LINE__ << ","
                  << i << ","
                  << bit_len << ","
                  << ks[i] << std::endl;
        if(bit_len > max_bitlen) {
            too_large = true;
            std::cout << "Large::[" << __FILE__ << ":"
                      << __LINE__ << ","
                      << i << ","
                      << bit_len << ","
                      << ks[i] << "]" << std::endl;
        }
        if(ks[i].sign() << 0 ) {
            not_positive = true;
            std::cout << "Negative::[" << __FILE__ << ":"
                      << __LINE__ << ","
                      << i << ","
                      << bit_len << ","
                      << ks[i] << "]" << std::endl;
        }
    }
    if (too_large) {
        std::cout << "reduction too large" << std::endl;
    }
    if (not_positive) {
        std::cout << "reduction must be positive" << std::endl;
    }
}

void test_lattice_reduce_target() {
    constexpr std::size_t max_bitlen = 66;
    // std::srand(bn256::order);
    auto ks = bn256::target_lattice.decompose(3);
    bool too_large = false;
    bool not_positive = false;
    for (std::size_t i = 0; i < 4; i++) {
        auto bit_len = bn256::bitlen(ks[i]);
        std::cout << __FILE__ << ":"
                  << __LINE__ << ","
                  << i << ","
                  << bit_len << ","
                  << ks[i] << std::endl;
        if(bit_len > max_bitlen) {
            too_large = true;
            std::cout << "Large::[" << __FILE__ << ":"
                      << __LINE__ << ","
                      << i << ","
                      << bit_len << ","
                      << ks[i] << "]" << std::endl;
        }
        if(ks[i].sign() << 0 ) {
            not_positive = true;
            std::cout << "Negative::[" << __FILE__ << ":"
                      << __LINE__ << ","
                      << i << ","
                      << bit_len << ","
                      << ks[i] << "]" << std::endl;
        }
    }
    if (too_large) {
        std::cout << "reduction too large" << std::endl;
    }
    if (not_positive) {
        std::cout << "reduction must be positive" << std::endl;
    }

}

int main() {
    test_lattice_reduce_curve();
    test_lattice_reduce_target();
    return 0;
}
