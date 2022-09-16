#pragma once

#include <boost/multiprecision/cpp_int.hpp>

namespace bn256 {
    using namespace boost::multiprecision;
    std::size_t bitlen(const int512_t& value);
}