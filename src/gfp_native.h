#pragma once
#include "array.h"
#include <tuple>

#if defined(__cpp_lib_is_constant_evaluated)
#   define BN256_IS_CONSTANT_EVALUATED std::is_constant_evaluated()
#elif (defined(__clang__) && (__clang_major__ > 8)) || (defined(__GNUC__) && (__GNUC__ > 8))
#   define BN256_IS_CONSTANT_EVALUATED __builtin_is_constant_evaluated()
#else
#   define BN256_IS_CONSTANT_EVALUATED true
#endif


// forward declarations
namespace bn256 {
namespace native {
   array<uint64_t, 4> gfp_carry(const array<uint64_t, 4>& a, uint64_t head) noexcept;
   array<uint64_t, 4> gfp_neg(const array<uint64_t, 4> a) noexcept;
   array<uint64_t, 4> gfp_add(const array<uint64_t, 4> a, const array<uint64_t, 4> b) noexcept;
   array<uint64_t, 4> gfp_sub(const array<uint64_t, 4> a, const array<uint64_t, 4> b) noexcept;
   array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept;
} // namespace native

namespace constants {
   // p2 is p, represented as little-endian 64-bit words.
   inline constexpr array<uint64_t, 4> p2 = { 0x3c208c16d87cfd47, 0x97816a916871ca8d, 0xb85045b68181585d,
                                              0x30644e72e131a029 };

   // np is the negative inverse of p, mod 2^256.
   inline constexpr array<uint64_t, 4> np = { 0x87d20782e4866389, 0x9ede7d651eca6ac9, 0xd8afcbd01833da80,
                                              0xf57a22b791888c6b };

} // namespace constants
} // namespace bn256

#if __x86_64__
#   include "gfp_amd64.h"
#endif

#ifndef BN256_NATIVE_ENABLED
#   define BN256_NATIVE_ENABLED 0
#endif

#define BN256_ALLOW_GFP_NATIVE (BN256_NATIVE_ENABLED && !BN256_IS_CONSTANT_EVALUATED)
