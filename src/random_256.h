#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <random>

namespace bn256 {

class random_256 {
 public:
   random_256();

   std::array<uint64_t, 4> sample();

 private:
   using generator = boost::random::independent_bits_engine<boost::random::mt19937_64,
                                                            std::numeric_limits<boost::multiprecision::uint256_t>::digits, boost::multiprecision::uint256_t>;
   boost::random::uniform_int_distribution<boost::multiprecision::uint256_t> uniform_int_distribution_;
   generator                                          gen_;
};
} // namespace bn256
