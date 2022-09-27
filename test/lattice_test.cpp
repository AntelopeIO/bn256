#include <lattice.h>
#include <iostream>
#include <iosfwd>
#include <bitlen.h>
#include <random_256.h>

using namespace boost::multiprecision::literals;
using namespace boost::multiprecision;

constexpr int test_fail = 1;
constexpr int test_pass = 0;

int test_lattice_reduce_curve() {
    int rc = test_pass;
    constexpr std::size_t max_bitlen = 130;
    bn256::random_256 rand;
    auto k = rand.sample();
    std::cout << "test_lattice_reduce_curve: " << k << std::endl;
    auto ks = bn256::curve_lattice.decompose(k);
    bool too_large = false;
    bool not_positive = false;
    for (std::size_t i = 0; i < 2; i++) {
        auto bit_len = bn256::bitlen(ks[i]);
        std::cout << i << ","
                  << ks[i] << ","
                  << bit_len << ","
                  << ks[i].sign() << std::endl;
        if(bit_len > max_bitlen) {
            too_large = true;
        }
        if(ks[i].sign() < 0 ) {
            not_positive = true;
        }
    }
    if (too_large) {
        std::cout << "reduction too large" << std::endl;
        rc = test_fail;
    }
    if (not_positive) {
        std::cout << "reduction must be positive" << std::endl;
        rc = test_fail;
    }
    std::cout << std::endl << std::endl;
    return rc;
}

int test_lattice_reduce_target() {
    int rc = test_pass;
    constexpr std::size_t max_bitlen = 66;
    bn256::random_256 rand;
    auto k = rand.sample();
    std::cout << "test_lattice_reduce_target: " << k << std::endl;
    auto ks = bn256::target_lattice.decompose(k);
    bool too_large = false;
    bool not_positive = false;
    for (std::size_t i = 0; i < 4; i++) {
        auto bit_len = bn256::bitlen(ks[i]);
        std::cout << i << ","
                  << ks[i] << ","
                  << bit_len << ","
                  << ks[i].sign() << std::endl;
        if(bit_len > max_bitlen) {
            too_large = true;
        }
        if(ks[i].sign() < 0 ) {
            not_positive = true;
        }
    }
    if (too_large) {
        std::cout << "reduction too large" << std::endl;
        rc = test_fail;
    }
    if (not_positive) {
        std::cout << "reduction must be positive" << std::endl;
        rc = test_fail;
    }
    std::cout << std::endl << std::endl;
    return rc;
}

int main() {
    int rc = test_pass;
    rc += test_lattice_reduce_curve();
    rc += test_lattice_reduce_target();
    return rc;
}
