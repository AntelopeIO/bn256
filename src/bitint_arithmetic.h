#pragma once

#include <cstdint>
#if !(defined(__BYTE_ORDER__) && __BYTE_ORDER__ == 1234)
#error This library only supports little endian architecture
#endif

#if defined(__cpp_lib_is_constant_evaluated)
#   define BN256_IS_CONSTANT_EVALUATED std::is_constant_evaluated()
#elif (defined(__clang__) && (__clang_major__ > 8)) || (defined(__GNUC__) && (__GNUC__ > 8))
#   define BN256_IS_CONSTANT_EVALUATED __builtin_is_constant_evaluated()
#else
#   define BN256_IS_CONSTANT_EVALUATED true
#endif

#if defined(__amd64__)
#   include <x86intrin.h>
#endif

namespace bn256 {
constexpr bool subborrow_u64(bool carry, uint64_t a, uint64_t b, uint64_t* c) noexcept {
#if defined(__amd64__)
   if (!BN256_IS_CONSTANT_EVALUATED) {
      return _subborrow_u64(carry, a, b, (unsigned long long*)c);
   }
#elif defined(__clang__)
   if (!BN256_IS_CONSTANT_EVALUATED) {
      unsigned long long carryout=0;
      *c = __builtin_subcll(a, b, carry, &carryout);
      return carryout;
   }
#endif

   carry = __builtin_sub_overflow(a, carry, c);
   carry |= __builtin_sub_overflow(*c, b, c);
   return carry;
}

constexpr bool addcarry_u64(bool carry, uint64_t a, uint64_t b, uint64_t* c) noexcept {
#if defined(__amd64__)
   if (!BN256_IS_CONSTANT_EVALUATED) {
      return _addcarry_u64(carry, a, b, (unsigned long long*)c);
   }
#elif defined(__clang__)
   if (!BN256_IS_CONSTANT_EVALUATED) {
      unsigned long long carryout=0;
      *c = __builtin_addcll(a, b, carry, &carryout);
      return carryout;
   }
#endif

   carry = __builtin_add_overflow(a, carry, c);
   carry |= __builtin_add_overflow(*c, b, c);
   return carry;
}

constexpr uint64_t mulx_u64(uint64_t a, uint64_t b, uint64_t* hi) noexcept {
#ifdef __BMI2__
   if (!BN256_IS_CONSTANT_EVALUATED)
      return _mulx_u64(a, b, (unsigned long long*)hi);
#endif
   __uint128_t x = a;
   x *= b;
   *hi = static_cast<uint64_t>(x >> 64);
   return static_cast<uint64_t>(x);
}

constexpr bool subborrow_u256(bool carry, const uint64_t* a, const uint64_t* b, uint64_t* c) noexcept {
   carry = subborrow_u64(carry, a[0], b[0], &c[0]);
   carry = subborrow_u64(carry, a[1], b[1], &c[1]);
   carry = subborrow_u64(carry, a[2], b[2], &c[2]);
   carry = subborrow_u64(carry, a[3], b[3], &c[3]);
   return carry;
}


constexpr bool subborrow_u512(bool carry, const uint64_t* a, const uint64_t* b, uint64_t* c) noexcept {
   carry = subborrow_u256(carry, &a[0], &b[0], &c[0]);
   carry = subborrow_u256(carry, &a[4], &b[4], &c[4]);
   return carry;
}

constexpr bool addcarry_u256(bool carry, const uint64_t* a, const uint64_t* b, uint64_t* c) noexcept {
   carry = addcarry_u64(carry, a[0], b[0], &c[0]);
   carry = addcarry_u64(carry, a[1], b[1], &c[1]);
   carry = addcarry_u64(carry, a[2], b[2], &c[2]);
   carry = addcarry_u64(carry, a[3], b[3], &c[3]);
   return carry;
}


constexpr bool addcarry_u512(bool carry, const uint64_t* a, const uint64_t* b, uint64_t* c) noexcept {
   carry = addcarry_u256(carry, &a[0], &b[0], &c[0]);
   carry = addcarry_u256(carry, &a[4], &b[4], &c[4]);
   return carry;
}

[[gnu::always_inline]] [[gnu::hot]] constexpr void full_mul_u256(const uint64_t* a, const uint64_t* b,
                                                                 uint64_t* c) noexcept {
#ifdef BN256_HAS_EXTINT
   if (!BN256_IS_CONSTANT_EVALUATED) {
      using extint_t = _ExtInt(512);
      extint_t x = 0, y = 0;
      memcpy(&x, a, sizeof(uint64_t) * 4);
      memcpy(&y, b, sizeof(uint64_t) * 4);
      extint_t z = x * y;
      memcpy(c, &z, sizeof(uint64_t) * 8);
      return;
   }
#endif
   uint64_t ax = 0, bx = 0, dx = 0;
   bool     carry = false;

   c[5]  = 0;
   dx    = b[0];
   c[0]  = mulx_u64(dx, a[0], &c[1]);
   ax    = mulx_u64(dx, a[1], &c[2]);
   carry = addcarry_u64(false, c[1], ax, &c[1]);
   ax    = mulx_u64(dx, a[2], &c[3]);
   carry = addcarry_u64(carry, c[2], ax, &c[2]);
   ax    = mulx_u64(dx, a[3], &c[4]);
   carry = addcarry_u64(carry, c[3], ax, &c[3]);
   carry = addcarry_u64(carry, c[4], 0, &c[4]);
   carry = addcarry_u64(carry, c[5], 0, &c[5]);

   c[6]  = 0;
   dx    = b[1];
   ax    = mulx_u64(dx, a[0], &bx);
   carry = addcarry_u64(0, c[1], ax, &c[1]);
   carry = addcarry_u64(carry, c[2], bx, &c[2]);
   ax    = mulx_u64(dx, a[2], &bx);
   carry = addcarry_u64(carry, c[3], ax, &c[3]);
   carry = addcarry_u64(carry, c[4], bx, &c[4]);
   carry = addcarry_u64(carry, c[5], 0, &c[5]);
   ax    = mulx_u64(dx, a[1], &bx);
   carry = addcarry_u64(0, c[2], ax, &c[2]);
   carry = addcarry_u64(carry, c[3], bx, &c[3]);
   ax    = mulx_u64(dx, a[3], &bx);
   carry = addcarry_u64(carry, c[4], ax, &c[4]);
   carry = addcarry_u64(carry, c[5], bx, &c[5]);
   carry = addcarry_u64(carry, c[6], 0, &c[6]);

   c[7]  = 0;
   dx    = b[2];
   ax    = mulx_u64(dx, a[0], &bx);
   carry = addcarry_u64(0, c[2], ax, &c[2]);
   carry = addcarry_u64(carry, c[3], bx, &c[3]);
   ax    = mulx_u64(dx, a[2], &bx);
   carry = addcarry_u64(carry, c[4], ax, &c[4]);
   carry = addcarry_u64(carry, c[5], bx, &c[5]);
   carry = addcarry_u64(carry, c[6], 0, &c[6]);
   ax    = mulx_u64(dx, a[1], &bx);
   carry = addcarry_u64(0, c[3], ax, &c[3]);
   carry = addcarry_u64(carry, c[4], bx, &c[4]);
   ax    = mulx_u64(dx, a[3], &bx);
   carry = addcarry_u64(carry, c[5], ax, &c[5]);
   carry = addcarry_u64(carry, c[6], bx, &c[6]);
   carry = addcarry_u64(carry, c[7], 0, &c[7]);

   dx    = b[3];
   ax    = mulx_u64(dx, a[0], &bx);
   carry = addcarry_u64(0, c[3], ax, &c[3]);
   carry = addcarry_u64(carry, c[4], bx, &c[4]);
   ax    = mulx_u64(dx, a[2], &bx);
   carry = addcarry_u64(carry, c[5], ax, &c[5]);
   carry = addcarry_u64(carry, c[6], bx, &c[6]);
   carry = addcarry_u64(carry, c[7], 0, &c[7]);
   ax    = mulx_u64(dx, a[1], &bx);
   carry = addcarry_u64(0, c[4], ax, &c[4]);
   carry = addcarry_u64(carry, c[5], bx, &c[5]);
   ax    = mulx_u64(dx, a[3], &bx);
   carry = addcarry_u64(carry, c[6], ax, &c[6]);
   carry = addcarry_u64(carry, c[7], bx, &c[7]);
}

[[gnu::always_inline]] [[gnu::hot]]
constexpr void half_mul_u256(const uint64_t* a, const uint64_t* b, uint64_t* c) noexcept {
#ifdef BN256_HAS_EXTINT
   if (!BN256_IS_CONSTANT_EVALUATED) {
      using extint_t = _ExtInt(256);
      extint_t x = 0, y = 0;
      memcpy(&x, a, sizeof(uint64_t) * 4);
      memcpy(&y, b, sizeof(uint64_t) * 4);
      extint_t z = x * y;
      memcpy(c, &z, sizeof(uint64_t) * 4);
      return;
   }
#endif
   uint64_t ax = 0, bx = 0, dx = 0;
   bool     carry = false;

   dx    = b[0];
   c[0]  = mulx_u64(dx, a[0], &c[1]);
   ax    = mulx_u64(dx, a[1], &c[2]);
   carry = addcarry_u64(false, c[1], ax, &c[1]);
   ax    = mulx_u64(dx, a[2], &c[3]);
   carry = addcarry_u64(carry, c[2], ax, &c[2]);
   ax    = mulx_u64(dx, a[3], &bx);
   addcarry_u64(carry, c[3], ax, &c[3]);

   dx    = b[1];
   ax    = mulx_u64(dx, a[0], &bx);
   carry = addcarry_u64(0, c[1], ax, &c[1]);
   carry = addcarry_u64(carry, c[2], bx, &c[2]);
   ax    = mulx_u64(dx, a[2], &bx);
   carry = addcarry_u64(carry, c[3], ax, &c[3]);
   ax    = mulx_u64(dx, a[1], &bx);
   carry = addcarry_u64(0, c[2], ax, &c[2]);
   addcarry_u64(carry, c[3], bx, &c[3]);

   dx    = b[2];
   ax    = mulx_u64(dx, a[0], &bx);
   carry = addcarry_u64(0, c[2], ax, &c[2]);
   carry = addcarry_u64(carry, c[3], bx, &c[3]);
   ax    = mulx_u64(dx, a[1], &bx);
   addcarry_u64(0, c[3], ax, &c[3]);

   dx = b[3];
   ax = mulx_u64(dx, a[0], &bx);
   addcarry_u64(0, c[3], ax, &c[3]);
}

} // namespace bn256