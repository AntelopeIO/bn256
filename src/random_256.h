#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <random>
#include <boost/random.hpp>

namespace bn256 {

   using namespace boost::multiprecision::literals;
   using namespace boost::multiprecision;
   using namespace boost::random;

   typedef independent_bits_engine<mt19937_64, std::numeric_limits<uint256_t>::digits, uint256_t> generator;

   class random_256 {
   public:
      random_256();

      uint256_t sample();

   private:
      uniform_int_distribution<uint256_t> uniform_int_distribution_;
      generator gen_;

   };
}
