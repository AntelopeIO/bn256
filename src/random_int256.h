#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <random>
#include <boost/random.hpp>

namespace bn256 {

    using namespace boost::multiprecision::literals;
    using namespace boost::multiprecision;
    using namespace boost::random;

    typedef boost::random::independent_bits_engine<mt19937_64 , std::numeric_limits<uint256_t>::digits, uint256_t> generator;
    class random_int256 {
    public:
        random_int256();
        uint256_t sample();

    private:
        boost::random::uniform_int_distribution<uint256_t> ur_;
        generator gen_;

    };
}
