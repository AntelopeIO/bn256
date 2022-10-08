#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <random>

namespace bn256 {

using namespace boost::multiprecision::literals;
using namespace boost::multiprecision;

class random_256 {
 public:
   random_256();

   uint256_t sample();

 private:
   using generator = boost::random::independent_bits_engine<boost::random::mt19937_64,
                                                            std::numeric_limits<uint256_t>::digits, uint256_t>;
   boost::random::uniform_int_distribution<uint256_t> uniform_int_distribution_;
   generator                                          gen_;
};
} // namespace bn256
