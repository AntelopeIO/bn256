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
   constexpr uint64_t  mask16 = 0x0000ffff;
   constexpr uint64_t  mask32 = 0xffffffff;
   array<uint64_t, 32> buff{};

   for (auto i = 0U; i < a.size(); ++i) {
      const auto     ai = a[i];
      const uint64_t a0 = ai & mask16;
      const uint64_t a1 = (ai >> 16ull) & mask16;
      const uint64_t a2 = (ai >> 32ull) & mask16;
      const uint64_t a3 = (ai >> 48ull);
      for (std::size_t j = 0; j < b.size(); ++j) {
         const uint64_t bj  = b[j];
         const uint64_t b0  = bj & mask32;
         const uint64_t b2  = bj >> 32ull;
         const uint64_t off = 4ull * (i + j);
         buff[off + 0] += a0 * b0;
         buff[off + 1] += a1 * b0;
         buff[off + 2] += a2 * b0 + a0 * b2;
         buff[off + 3] += a3 * b0 + a1 * b2;
         buff[off + 4] += a2 * b2;
         buff[off + 5] += a3 * b2;
      }
   }
   for (uint64_t i = 1ull; i < 4ull; i++) {
      const auto shift = 16ull * i;
      uint64_t   head = 0ull, carry = 0ull;
      for (uint64_t j = 0ull; j < 8ull; j++) {
         const auto block = 4ull * j;
         const auto xi    = buff[block];
         const auto yi    = (buff[block + i] << shift) + head;
         const auto zi    = xi + yi + carry;
         buff[block]      = zi;
         carry            = ((xi & yi) | ((xi | yi) & ~zi)) >> 63ull;
         head             = buff[block + i] >> (64ull - shift);
      }
   }
   return { buff[0], buff[4], buff[8], buff[12], buff[16], buff[20], buff[24], buff[28] };
}

constexpr array<uint64_t, 4> half_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   constexpr uint64_t  mask16 = 0x0000ffff;
   constexpr uint64_t  mask32 = 0xffffffff;
   array<uint64_t, 18> buff{};
   for (auto i = 0U; i < a.size(); ++i) {
      const uint64_t ai = a[i];
      const uint64_t a0 = ai & mask16;
      const uint64_t a1 = (ai >> 16ull) & mask16;
      const uint64_t a2 = (ai >> 32ull) & mask16;
      const uint64_t a3 = (ai >> 48ull);
      for (std::size_t j = 0; j < b.size(); ++j) {
         const auto bj = b[j];
         if (i + j > 3) {
            break;
         }
         const uint64_t b0  = bj & mask32;
         const uint64_t b2  = bj >> 32ull;
         const uint64_t off = 4 * (i + j);
         buff[off + 0] += a0 * b0;
         buff[off + 1] += a1 * b0;
         buff[off + 2] += a2 * b0 + a0 * b2;
         buff[off + 3] += a3 * b0 + a1 * b2;
         buff[off + 4] += a2 * b2;
         buff[off + 5] += a3 * b2;
      }
   }
   for (uint64_t i = 1ull; i < 4ull; i++) {
      const uint64_t shift = 16ull * i;
      uint64_t       head = 0ull, carry = 0ull;
      for (uint64_t j = 0ull; j < 4ull; j++) {
         const uint64_t block = 4ull * j;
         const uint64_t xi    = buff[block];
         const uint64_t yi    = (buff[block + i] << shift) + head;
         const uint64_t zi    = xi + yi + carry;
         buff[block]          = zi;
         carry                = ((xi & yi) | ((xi | yi) & ~zi)) >> 63ull;
         head                 = buff[block + i] >> (64ull - shift);
      }
   }
   return { buff[0], buff[4], buff[8], buff[12] };
}

constexpr array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   auto T = full_mul(a, b);
   auto m = half_mul({ T[0], T[1], T[2], T[3] }, constants::np);
   auto t = full_mul(m, constants::p2);

   uint64_t carry = 0;
   for (auto i = 0U; i < T.size(); ++i) {
      const auto Ti = T[i];
      const auto ti = t[i];
      const auto zi = Ti + ti + carry;
      T[i]          = zi;
      carry         = ((Ti & ti) | ((Ti | ti) & ~zi)) >> 63ull;
   }

   return gfp_carry({ T[4], T[5], T[6], T[7] }, carry);
}

} // namespace bn256