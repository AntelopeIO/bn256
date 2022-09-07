#pragma once

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::int128_t;

namespace bn256 {
    std::size_t bitlen(const int128_t& value);
}