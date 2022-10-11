#pragma once

#include <boost/multiprecision/cpp_int.hpp>

namespace bn256 {
using namespace boost::multiprecision;

inline std::size_t bitlen(const int512_t& value) noexcept {
      std::size_t len = 0;
      auto x = abs(value);
      while (x) {
         len++;
         x >>= 1;
      }
      return len;
   }
} // namespace bn256