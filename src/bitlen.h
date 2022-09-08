#pragma once

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::int256_t;

namespace bn256 {
    std::size_t bitlen(const int256_t& value);
}