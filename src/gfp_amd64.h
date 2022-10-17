#pragma once

#include <x86intrin.h>
#define BN256_NATIVE_ENABLED 1
namespace bn256::native {
inline std::tuple<array<uint64_t, 4>, bool> sub_u256(const array<uint64_t, 4>& a,
                                                     const array<uint64_t, 4>& b) noexcept {
   array<uint64_t, 4> c;
   bool               carry = 0;
   carry                    = _subborrow_u64(carry, a[0], b[0], (unsigned long long*)&c[0]);
   carry                    = _subborrow_u64(carry, a[1], b[1], (unsigned long long*)&c[1]);
   carry                    = _subborrow_u64(carry, a[2], b[2], (unsigned long long*)&c[2]);
   carry                    = _subborrow_u64(carry, a[3], b[3], (unsigned long long*)&c[3]);
   return { c, carry };
}

inline std::tuple<array<uint64_t, 4>, bool> add_u256(const array<uint64_t, 4>& a,
                                                     const array<uint64_t, 4>& b) noexcept {
   array<uint64_t, 4> c;
   bool               carry = 0;
   carry                    = _addcarry_u64(carry, a[0], b[0], (unsigned long long*)&c[0]);
   carry                    = _addcarry_u64(carry, a[1], b[1], (unsigned long long*)&c[1]);
   carry                    = _addcarry_u64(carry, a[2], b[2], (unsigned long long*)&c[2]);
   carry                    = _addcarry_u64(carry, a[3], b[3], (unsigned long long*)&c[3]);
   return { c, carry };
}

inline array<uint64_t, 4> gfp_carry(const array<uint64_t, 4>& a, uint64_t head) noexcept {
   auto [b, carry] = sub_u256(a, constants::p2);
   carry           = _subborrow_u64(carry, head, 0, (unsigned long long*)&head);
   return carry ? a : b;
}

inline array<uint64_t, 4> gfp_neg(const array<uint64_t, 4> a) noexcept {
   auto [b, _] = sub_u256(constants::p2, a);
   return gfp_carry(b, 0);
}

inline array<uint64_t, 4> gfp_add(const array<uint64_t, 4> a, const array<uint64_t, 4> b) noexcept {
   auto [c, carry] = add_u256(a, b);
   return gfp_carry(c, carry);
}

inline array<uint64_t, 4> gfp_sub(const array<uint64_t, 4> a, const array<uint64_t, 4> b) noexcept {
   auto [t, _]     = sub_u256(constants::p2, b);
   auto [c, carry] = add_u256(a, t);
   return gfp_carry(c, carry);
}

#if (defined(__clang__) && (__clang_major__ > 11) && (__clang_major__<14 || defined(__APPLE_CC__) && (__clang_major__<=14)))
template <typename T, typename U>
T bit_cast(U u) {
    T r{};
    memcpy(&r, &u, sizeof(u));
    return r;
}

inline array<uint64_t, 8> full_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   using extint_t = _ExtInt(512);
    extint_t A = bit_cast<extint_t>(a);
    extint_t B = bit_cast<extint_t>(b);
    return bit_cast<array<uint64_t,8>>(A*B);
}

inline array<uint64_t, 4> half_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   using extint_t = _ExtInt(256);
    extint_t A = bit_cast<extint_t>(a);
    extint_t B = bit_cast<extint_t>(b);
    return bit_cast<array<uint64_t,4>>(A*B);
}
#elif defined(__BMI2__)
inline array<uint64_t, 8> full_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   array<uint64_t, 8> c;
   c[5] = 0;
   c[0] = _mulx_u64(a[0], b[0], (unsigned long long*)&c[1]);
   unsigned long long ax, bx;
   ax         = _mulx_u64(a[0], b[1], (unsigned long long*)&c[2]);
   bool carry = _addcarry_u64(0, c[1], ax, (unsigned long long*)&c[1]);
   ax         = _mulx_u64(a[0], b[2], (unsigned long long*)&c[3]);
   carry      = _addcarry_u64(carry, c[2], ax, (unsigned long long*)&c[2]);
   ax         = _mulx_u64(a[0], b[3], (unsigned long long*)&c[4]);
   carry      = _addcarry_u64(carry, c[3], ax, (unsigned long long*)&c[3]);
   carry      = _addcarry_u64(carry, c[4], 0, (unsigned long long*)&c[4]);
   carry      = _addcarry_u64(carry, c[5], 0, (unsigned long long*)&c[5]);

   c[6]  = 0;
   ax    = _mulx_u64(a[1], b[0], &bx);
   carry = _addcarry_u64(0, c[1], ax, (unsigned long long*)&c[1]);
   carry = _addcarry_u64(carry, c[2], bx, (unsigned long long*)&c[2]);
   ax    = _mulx_u64(a[1], b[2], &bx);
   carry = _addcarry_u64(carry, c[3], ax, (unsigned long long*)&c[3]);
   carry = _addcarry_u64(carry, c[4], bx, (unsigned long long*)&c[4]);
   carry = _addcarry_u64(carry, c[5], 0, (unsigned long long*)&c[5]);
   ax    = _mulx_u64(a[1], b[1], &bx);
   carry = _addcarry_u64(0, c[2], ax, (unsigned long long*)&c[2]);
   carry = _addcarry_u64(carry, c[3], bx, (unsigned long long*)&c[3]);
   ax    = _mulx_u64(a[1], b[3], &bx);
   carry = _addcarry_u64(carry, c[4], ax, (unsigned long long*)&c[4]);
   carry = _addcarry_u64(carry, c[5], bx, (unsigned long long*)&c[5]);
   carry = _addcarry_u64(carry, c[6], 0, (unsigned long long*)&c[6]);

   c[7]  = 0;
   ax    = _mulx_u64(a[2], b[0], &bx);
   carry = _addcarry_u64(0, c[2], ax, (unsigned long long*)&c[2]);
   carry = _addcarry_u64(carry, c[3], bx, (unsigned long long*)&c[3]);
   ax    = _mulx_u64(a[2], b[2], &bx);
   carry = _addcarry_u64(carry, c[4], ax, (unsigned long long*)&c[4]);
   carry = _addcarry_u64(carry, c[5], bx, (unsigned long long*)&c[5]);
   carry = _addcarry_u64(carry, c[6], 0, (unsigned long long*)&c[6]);
   ax    = _mulx_u64(a[2], b[1], &bx);
   carry = _addcarry_u64(0, c[3], ax, (unsigned long long*)&c[3]);
   carry = _addcarry_u64(carry, c[4], bx, (unsigned long long*)&c[4]);
   ax    = _mulx_u64(a[2], b[3], &bx);
   carry = _addcarry_u64(carry, c[5], ax, (unsigned long long*)&c[5]);
   carry = _addcarry_u64(carry, c[6], bx, (unsigned long long*)&c[6]);
   carry = _addcarry_u64(carry, c[7], 0, (unsigned long long*)&c[7]);

   ax    = _mulx_u64(a[3], b[0], &bx);
   carry = _addcarry_u64(0, c[3], ax, (unsigned long long*)&c[3]);
   carry = _addcarry_u64(carry, c[4], bx, (unsigned long long*)&c[4]);
   ax    = _mulx_u64(a[3], b[2], &bx);
   carry = _addcarry_u64(carry, c[5], ax, (unsigned long long*)&c[5]);
   carry = _addcarry_u64(carry, c[6], bx, (unsigned long long*)&c[6]);
   carry = _addcarry_u64(carry, c[7], 0, (unsigned long long*)&c[7]);
   ax    = _mulx_u64(a[3], b[1], &bx);
   carry = _addcarry_u64(0, c[4], ax, (unsigned long long*)&c[4]);
   carry = _addcarry_u64(carry, c[5], bx, (unsigned long long*)&c[5]);
   ax    = _mulx_u64(a[3], b[3], &bx);
   carry = _addcarry_u64(carry, c[6], ax, (unsigned long long*)&c[6]);
   carry = _addcarry_u64(carry, c[7], bx, (unsigned long long*)&c[7]);
   return c;
}

inline array<uint64_t, 4> half_mul(array<uint64_t, 4> a, array<uint64_t, 4> b) noexcept {
   array<uint64_t, 4> c = {};
   unsigned long long ax=0, bx=0;

   c[0] = _mulx_u64(a[0], b[0], (unsigned long long*)&c[1]);
   ax         = _mulx_u64(a[0], b[1], (unsigned long long*)&c[2]);
   bool carry = _addcarry_u64(0, c[1], ax, (unsigned long long*)&c[1]);
   ax         = _mulx_u64(a[0], b[2], (unsigned long long*)&c[3]);
   carry      = _addcarry_u64(carry, c[2], ax, (unsigned long long*)&c[2]);
   ax         = _mulx_u64(a[0], b[3], &bx);
   _addcarry_u64(carry, c[3], ax, (unsigned long long*)&c[3]);

   ax    = _mulx_u64(a[1], b[0], &bx);
   carry = _addcarry_u64(0, c[1], ax, (unsigned long long*)&c[1]);
   carry = _addcarry_u64(carry, c[2], bx, (unsigned long long*)&c[2]);
   ax    = _mulx_u64(a[1], b[2], &bx);
   carry = _addcarry_u64(carry, c[3], ax, (unsigned long long*)&c[3]);
   ax    = _mulx_u64(a[1], b[1], &bx);
   carry = _addcarry_u64(0, c[2], ax, (unsigned long long*)&c[2]);
   _addcarry_u64(carry, c[3], bx, (unsigned long long*)&c[3]);

   ax    = _mulx_u64(a[2], b[0], &bx);
   carry = _addcarry_u64(0, c[2], ax, (unsigned long long*)&c[2]);
   carry = _addcarry_u64(carry, c[3], bx, (unsigned long long*)&c[3]);
   ax    = _mulx_u64(a[2], b[1], &bx);
   _addcarry_u64(0, c[3], ax, (unsigned long long*)&c[3]);

   ax    = _mulx_u64(a[3], b[0], &bx);
   _addcarry_u64(0, c[3], ax, (unsigned long long*)&c[3]);

   return c;
}
#else
#error "Please compile the code with -mbmi2 option"
#endif

inline array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   auto T = full_mul(a, b);
   auto m = half_mul({ T[0], T[1], T[2], T[3] }, constants::np);
   auto t = full_mul(m, constants::p2);

   bool carry = 0;
#pragma GCC unroll 8
   for (int i = 0; i < 8; ++i) { carry = _addcarry_u64(carry, T[i], t[i], (unsigned long long*)&T[i]); }
   return gfp_carry({ T[4], T[5], T[6], T[7] }, carry);
}

} // namespace bn256::native
