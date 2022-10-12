#pragma once

#include <boost/multiprecision/cpp_int.hpp>
namespace bn256 {
using namespace boost::multiprecision;

inline std::size_t bitlen(const int512_t& value) noexcept {
   return value == 0 ? 0 : msb(abs(value)) + 1;
}

} // namespace bn256