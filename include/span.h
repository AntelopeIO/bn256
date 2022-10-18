#pragma once

#ifdef __cpp_lib_span
#   include <span>
#else
#   include <nonstd/span.hpp>
namespace std {
template <class T, std::size_t Extent>
using span = nonstd::span<T, Extent>;
} // namespace std
#endif
