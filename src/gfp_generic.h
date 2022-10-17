#pragma once
#include <cstddef>
#include <cstdint>
#include "gfp_native.h"
namespace bn256 {



constexpr array<uint64_t, 4> gfp_carry(const array<uint64_t, 4>& a, uint64_t head) noexcept {

   if (BN256_ALLOW_GFP_NATIVE) {
      return native::gfp_carry(a, head);
   }

   array<uint64_t, 4> b{};
   uint64_t           carry = {};

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

   if (BN256_ALLOW_GFP_NATIVE) {
      return native::gfp_neg(a);
   }

   uint64_t           carry = 0;
   array<uint64_t, 4> b{};

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

   if (BN256_ALLOW_GFP_NATIVE) {
      return native::gfp_add(a, b);
   }

   uint64_t           carry = 0;
   array<uint64_t, 4> c{};

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

   if (BN256_ALLOW_GFP_NATIVE) {
      return native::gfp_sub(a, b);
   }

   uint64_t           carry = 0;
   array<uint64_t, 4> t{};

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
   auto add_to_c = [&c](__int128_t x, int index) {
        uint64_t upper = x >> 64;
        uint64_t carry = __builtin_add_overflow( c[index], static_cast<uint64_t>(x), &c[index]);
        carry = __builtin_add_overflow(c[index+1], carry, &c[index+1]);
        carry += __builtin_add_overflow(upper, c[index+1], &c[index+1]);
        __builtin_add_overflow(c[index+2], carry, &c[index+2]);
    };

   for (int i = 0; i < 4; ++i)
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

   for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j) {
         if ((i + j) > 3)
            break;
         add_to_c(static_cast<__uint128_t>(b[i]) * a[j], i + j);
      }
   return { c[0], c[1], c[2], c[3] };
}

constexpr array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {

   if (BN256_ALLOW_GFP_NATIVE) {
      return native::gfp_mul(a, b);
   }

   auto T = full_mul(a, b);
   auto m = half_mul({ T[0], T[1], T[2], T[3] }, constants::np);
   auto t = full_mul(m, constants::p2);

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