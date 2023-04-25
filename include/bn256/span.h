#pragma once

#if __cplusplus > 201703L
#   include <span>
#else
#   include <limits>
#   include "nonstd/span.hpp"
namespace std {
template <class T, std::size_t Extent= std::numeric_limits<std::size_t>::max()>
using span = nonstd::span<T, Extent>;
} // namespace std
#endif
