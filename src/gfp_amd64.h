#pragma once

#include <x86intrin.h>
#include <cstring>
#define BN256_NATIVE_ENABLED 1

extern "C" {
   struct u256_t {
      unsigned long long values[4];
   };

   struct u256_t gfp_neg(struct u256_t a);
   struct u256_t gfp_add(struct u256_t a, struct u256_t b);
   struct u256_t gfp_sub(struct u256_t a, struct u256_t b);
   struct u256_t gfp_mul(struct u256_t a, struct u256_t b);
}

namespace bn256::native {

template <typename T, typename U>
T bit_cast(U u) {
    T r{};
    memcpy(&r, &u, sizeof(u));
    return r;
}

#if defined(__clang__)
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

#if defined(HAVE_EXTINT)

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

inline array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {

   auto T = full_mul(a, b);
   auto m = half_mul({ T[0], T[1], T[2], T[3] }, constants::np);
   auto t = full_mul(m, constants::p2);

   bool carry = 0;
#pragma GCC unroll 8
   for (int i = 0; i < 8; ++i) { carry = _addcarry_u64(carry, T[i], t[i], (unsigned long long*)&T[i]); }
   return gfp_carry({ T[4], T[5], T[6], T[7] }, carry);
}

#else
inline array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   return bit_cast<array<uint64_t, 4>>(::gfp_mul(bit_cast<u256_t>(a), bit_cast<u256_t>(b)));
}
#endif

#else // GCC

inline array<uint64_t, 4> gfp_neg(const array<uint64_t, 4> a) noexcept {
   return bit_cast<array<uint64_t, 4>>(::gfp_neg(bit_cast<u256_t>(a)));
}

inline array<uint64_t, 4> gfp_add(const array<uint64_t, 4> a, const array<uint64_t, 4> b) noexcept {
   return bit_cast<array<uint64_t, 4>>(::gfp_add(bit_cast<u256_t>(a), bit_cast<u256_t>(b)));
}

inline array<uint64_t, 4> gfp_sub(const array<uint64_t, 4> a, const array<uint64_t, 4> b) noexcept {
   return bit_cast<array<uint64_t, 4>>(::gfp_sub(bit_cast<u256_t>(a), bit_cast<u256_t>(b)));
}

inline array<uint64_t, 4> gfp_mul(const array<uint64_t, 4>& a, const array<uint64_t, 4>& b) noexcept {
   return bit_cast<array<uint64_t, 4>>(::gfp_mul(bit_cast<u256_t>(a), bit_cast<u256_t>(b)));
}
#endif



} // namespace bn256::native
