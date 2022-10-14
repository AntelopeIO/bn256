#pragma once
#include "array.h"
#include <cstddef>
#include <cstdint>
namespace bn256 {

namespace constants {
   // p2 is p, represented as little-endian 64-bit words.
   inline constexpr array<uint64_t, 4> p2 = { 0x3c208c16d87cfd47, 0x97816a916871ca8d, 0xb85045b68181585d,
                                              0x30644e72e131a029 };

   // np is the negative inverse of p, mod 2^256.
   inline constexpr array<uint64_t, 4> np = { 0x87d20782e4866389, 0x9ede7d651eca6ac9, 0xd8afcbd01833da80,
                                              0xf57a22b791888c6b };

} // namespace constants

constexpr array<uint64_t, 4> gfp_carry(const array<uint64_t, 4>& a, uint64_t head) noexcept {
   array<uint64_t, 4> b{};

#if defined(__clang__) && __clang_major__ > 8
   if (!__builtin_is_constant_evaluated()) {
      unsigned long long carryin = 0, carryout = 0;
      b[0]    = __builtin_subcll(a[0], constants::p2[0], carryin, &carryout);
      carryin = carryout;
      b[1]    = __builtin_subcll(a[1], constants::p2[1], carryin, &carryout);
      carryin = carryout;
      b[2]    = __builtin_subcll(a[2], constants::p2[2], carryin, &carryout);
      carryin = carryout;
      b[3]    = __builtin_subcll(a[3], constants::p2[3], carryin, &carryout);
      carryin = carryout;
      __builtin_subcll(head, 0, carryin, &carryout);

      return carryout ? a : b;
   }
#endif

   uint64_t carry = {};

   carry = __builtin_sub_overflow(a[0], constants::p2[0], &b[0]);
   carry = __builtin_sub_overflow(a[1], carry, &b[1]);
   carry += __builtin_sub_overflow(b[1], constants::p2[1], &b[1]);
   carry = __builtin_sub_overflow(a[2], carry, &b[2]);
   carry += __builtin_sub_overflow(b[2], constants::p2[2], &b[2]);
   carry = __builtin_sub_overflow(a[3], carry, &b[3]);
   carry += __builtin_sub_overflow(b[3], constants::p2[3], &b[3]);
   carry = __builtin_sub_overflow(head, carry, &head);

   return carry ? a : b;
}

constexpr array<uint64_t, 4> gfp_neg(const array<uint64_t, 4>& a) noexcept {
   array<uint64_t, 4> b{};
#if defined(__clang__) && __clang_major__ > 8
   if (!__builtin_is_constant_evaluated()) {
      unsigned long long carryin = 0, carryout = 0;
      b[0]    = __builtin_subcll(constants::p2[0], a[0], carryin, &carryout);
      carryin = carryout;
      b[1]    = __builtin_subcll(constants::p2[1], a[1], carryin, &carryout);
      carryin = carryout;
      b[2]    = __builtin_subcll(constants::p2[2], a[2], carryin, &carryout);
      carryin = carryout;
      b[3]    = __builtin_subcll(constants::p2[3], a[3], carryin, &carryout);
      return gfp_carry(b, 0);
   }
#endif
   uint64_t carry = 0;

   carry = __builtin_sub_overflow(constants::p2[0], a[0], &b[0]);
   carry = __builtin_sub_overflow(constants::p2[1], carry, &b[1]);
   carry += __builtin_sub_overflow(b[1], a[1], &b[1]);
   carry = __builtin_sub_overflow(constants::p2[2], carry, &b[2]);
   carry += __builtin_sub_overflow(b[2], a[2], &b[2]);
   carry = __builtin_sub_overflow(constants::p2[3], carry, &b[3]);
   carry += __builtin_sub_overflow(b[3], a[3], &b[3]);
   return gfp_carry(b, 0);
}

constexpr array<uint64_t, 4> gfp_add(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   array<uint64_t, 4> c{};

#if defined(__clang__) && __clang_major__ > 8
   if (!__builtin_is_constant_evaluated()) {
      unsigned long long carryin = 0, carryout = 0;
      c[0]    = __builtin_addcll(a[0], b[0], carryin, &carryout);
      carryin = carryout;
      c[1]    = __builtin_addcll(a[1], b[1], carryin, &carryout);
      carryin = carryout;
      c[2]    = __builtin_addcll(a[2], b[2], carryin, &carryout);
      carryin = carryout;
      c[3]    = __builtin_addcll(a[3], b[3], carryin, &carryout);
      return gfp_carry(c, carryout);
   }
#endif
   uint64_t carry = 0;

   carry = __builtin_add_overflow(a[0], b[0], &c[0]);
   carry = __builtin_add_overflow(a[1], carry, &c[1]);
   carry += __builtin_add_overflow(c[1], b[1], &c[1]);
   carry = __builtin_add_overflow(a[2], carry, &c[2]);
   carry += __builtin_add_overflow(c[2], b[2], &c[2]);
   carry = __builtin_add_overflow(a[3], carry, &c[3]);
   carry += __builtin_add_overflow(c[3], b[3], &c[3]);

   return gfp_carry(c, carry);
}

constexpr array<uint64_t, 4> gfp_sub(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   array<uint64_t, 4> t{};

#if defined(__clang__) && __clang_major__ > 8
   if (!__builtin_is_constant_evaluated()) {
      unsigned long long carryin = 0, carryout = 0;
      t[0]    = __builtin_subcll(constants::p2[0], b[0], carryin, &carryout);
      carryin = carryout;
      t[1]    = __builtin_subcll(constants::p2[1], b[1], carryin, &carryout);
      carryin = carryout;
      t[2]    = __builtin_subcll(constants::p2[2], b[2], carryin, &carryout);
      carryin = carryout;
      t[3]    = __builtin_subcll(constants::p2[3], b[3], carryin, &carryout);

      carryin = 0;
      array<uint64_t, 4> c{};

      c[0]    = __builtin_addcll(a[0], t[0], carryin, &carryout);
      carryin = carryout;
      c[1]    = __builtin_addcll(a[1], t[1], carryin, &carryout);
      carryin = carryout;
      c[2]    = __builtin_addcll(a[2], t[2], carryin, &carryout);
      carryin = carryout;
      c[3]    = __builtin_addcll(a[3], t[3], carryin, &carryout);

      return gfp_carry(c, carryout);
   }
#endif
   uint64_t carry = 0;

   carry = __builtin_sub_overflow(constants::p2[0], b[0], &t[0]);
   carry = __builtin_sub_overflow(constants::p2[1], carry, &t[1]);
   carry += __builtin_sub_overflow(t[1], b[1], &t[1]);
   carry = __builtin_sub_overflow(constants::p2[2], carry, &t[2]);
   carry += __builtin_sub_overflow(t[2], b[2], &t[2]);
   carry = __builtin_sub_overflow(constants::p2[3], carry, &t[3]);
   __builtin_sub_overflow(t[3], b[3], &t[3]);

   carry = 0;
   array<uint64_t, 4> c{};

   carry = __builtin_add_overflow(a[0], t[0], &c[0]);
   carry = __builtin_add_overflow(a[1], carry, &c[1]);
   carry += __builtin_add_overflow(c[1], t[1], &c[1]);
   carry = __builtin_add_overflow(a[2], carry, &c[2]);
   carry += __builtin_add_overflow(c[2], t[2], &c[2]);
   carry = __builtin_add_overflow(a[3], carry, &c[3]);
   carry += __builtin_add_overflow(c[3], t[3], &c[3]);

   return gfp_carry(c, carry);
}

constexpr array<uint64_t, 8> full_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   array<uint64_t, 9> c{};
   auto               add_to_c = [&c](__int128_t x, int index) {
      uint64_t upper = x >> 64;
      uint64_t carry = __builtin_add_overflow(c[index], static_cast<uint64_t>(x), &c[index]);
      carry          = __builtin_add_overflow(c[index + 1], carry, &c[index + 1]);
      carry += __builtin_add_overflow(upper, c[index + 1], &c[index + 1]);
      __builtin_add_overflow(c[index + 2], carry, &c[index + 2]);
   };

   #pragma GCC unroll 4
   for (int i = 0; i < 4; ++i)
   #pragma GCC unroll 4
      for (int j = 0; j < 4; ++j) { add_to_c(static_cast<__uint128_t>(b[i]) * a[j], i + j); }
   return { c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7] };
}

constexpr array<uint64_t, 4> half_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   array<uint64_t, 6> c{};
   auto               add_to_c = [&c](__int128_t x, int index) {
      uint64_t upper = x >> 64;
      uint64_t carry = __builtin_add_overflow(c[index], static_cast<uint64_t>(x), &c[index]);
      carry          = __builtin_add_overflow(c[index + 1], carry, &c[index + 1]);
      carry += __builtin_add_overflow(upper, c[index + 1], &c[index + 1]);
      __builtin_add_overflow(c[index + 2], carry, &c[index + 2]);
   };

   #pragma GCC unroll 4
   for (int i = 0; i < 4; ++i)
   #pragma GCC unroll 4
      for (int j = 0; j < 4; ++j) {
         if ((i + j) > 3)
            break;
         add_to_c(static_cast<__uint128_t>(b[i]) * a[j], i + j);
      }
   return { c[0], c[1], c[2], c[3] };
}

constexpr array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   auto T = full_mul(a, b);
   auto m = half_mul({ T[0], T[1], T[2], T[3] }, constants::np);
   auto t = full_mul(m, constants::p2);

#if defined(__clang__) && __clang_major__ > 8
   if (!__builtin_is_constant_evaluated()) {
      unsigned long long carryin = 0, carryout = 0;
      T[0]    = __builtin_addcll(T[0], t[0], carryin, &carryout);
      carryin = carryout;
      T[1]    = __builtin_addcll(T[1], t[1], carryin, &carryout);
      carryin = carryout;
      T[2]    = __builtin_addcll(T[2], t[2], carryin, &carryout);
      carryin = carryout;
      T[3]    = __builtin_addcll(T[3], t[3], carryin, &carryout);
      carryin = carryout;
      T[4]    = __builtin_addcll(T[4], t[4], carryin, &carryout);
      carryin = carryout;
      T[5]    = __builtin_addcll(T[5], t[5], carryin, &carryout);
      carryin = carryout;
      T[6]    = __builtin_addcll(T[6], t[6], carryin, &carryout);
      carryin = carryout;
      T[7]    = __builtin_addcll(T[7], t[7], carryin, &carryout);
      return gfp_carry({ T[4], T[5], T[6], T[7] }, carryout);
   }
#endif

   uint64_t carry = 0;

   carry = __builtin_add_overflow(T[0], t[0], &T[0]);
   carry = __builtin_add_overflow(T[1], carry, &T[1]);
   carry += __builtin_add_overflow(T[1], t[1], &T[1]);
   carry = __builtin_add_overflow(T[2], carry, &T[2]);
   carry += __builtin_add_overflow(T[2], t[2], &T[2]);
   carry = __builtin_add_overflow(T[3], carry, &T[3]);
   carry += __builtin_add_overflow(T[3], t[3], &T[3]);
   carry = __builtin_add_overflow(T[4], carry, &T[4]);
   carry = __builtin_add_overflow(T[4], t[4], &T[4]);
   carry = __builtin_add_overflow(T[5], carry, &T[5]);
   carry += __builtin_add_overflow(T[5], t[5], &T[5]);
   carry = __builtin_add_overflow(T[6], carry, &T[6]);
   carry += __builtin_add_overflow(T[6], t[6], &T[6]);
   carry = __builtin_add_overflow(T[7], carry, &T[7]);
   __builtin_add_overflow(T[7], t[7], &T[7]);

   return gfp_carry({ T[4], T[5], T[6], T[7] }, carry);
}

} // namespace bn256