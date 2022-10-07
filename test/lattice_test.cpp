#include <lattice.h>
#include <iostream>
#include <iosfwd>
#include <bitlen.h>
#include <random_256.h>
#include <catch2/catch_test_macros.hpp>

using namespace boost::multiprecision::literals;
using namespace boost::multiprecision;

constexpr int test_fail = 1;
constexpr int test_pass = 0;

TEST_CASE("test lattice reduce curve", "[test_lattice_reduce_curve]"){
    constexpr std::size_t max_bitlen = 130;
    bn256::random_256 rand;
    auto k = rand.sample();
    auto ks = bn256::curve_lattice.decompose(k);

    for (std::size_t i = 0; i < 2; i++) {
        CHECK(bn256::bitlen(ks[i]) <= max_bitlen ); // fails if reduction too large
        CHECK(ks[i].sign() >= 0); // reduction must be positive
    }
}

TEST_CASE("test lattice reduce target", "[lattice_reduce_target]"){

    constexpr std::size_t max_bitlen = 66;
    bn256::random_256 rand;
    auto k = rand.sample();
    auto ks = bn256::target_lattice.decompose(k);

    for (std::size_t i = 0; i < 4; i++) {
        CHECK(bn256::bitlen(ks[i]) <= max_bitlen ); // fails if reduction too large
        CHECK(ks[i].sign() >= 0); // reduction must be positive
    }
}
