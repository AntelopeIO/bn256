#pragma once

#if defined(__cpp_lib_span) || (__cplusplus >= 202002L && defined(__has_include) && __has_include(<span>))
#   include <span>
#else
#   include <limits>
#   include "nonstd/span.hpp"
namespace std {
template <class T, std::size_t Extent= std::numeric_limits<std::size_t>::max()>
using span = nonstd::span<T, Extent>;
} // namespace std
#endif
