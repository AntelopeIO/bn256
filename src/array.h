#pragma once
#include <cstddef>
#if __cplusplus > 201703L
#   include <array>
namespace bn256 {
template <typename T, std::size_t S>
using array = std::array<T, S>;
}
#else
#include <span.h>

namespace bn256 {
// std::array in C++17 cannot be used in constexpr context; therefore we rollout our own.
template <typename T, std::size_t N>
struct array {
   T                     v_[N];
   constexpr T&          operator[](std::size_t i) noexcept { return v_[i]; }
   constexpr const T&    operator[](std::size_t i) const noexcept { return v_[i]; }
   constexpr std::size_t size() const noexcept { return N; }
   constexpr T*          data() noexcept { return v_; }
   constexpr const T*    data() const noexcept { return v_; }

   constexpr const T*    begin() const noexcept { return v_; }
   constexpr const T*    end() const noexcept { return v_+N; }

   constexpr bool operator==(const array<T, N>& other) const noexcept {
      for (std::size_t i = 0; i < N; ++i)
         if (v_[i] != other[i])
            return false;
      return true;
   }

   constexpr bool operator!=(const array& other) const noexcept { return !(*this == other); }

   constexpr operator std::span<T,N> () noexcept {
      return std::span<T,N> {v_};
   }

   constexpr operator std::span<const T,N> () const noexcept {
      return std::span<const T,N> {v_};
   }
};
} // namespace bn256
#endif

namespace bn256 {
template <std::size_t N>
constexpr int bitlen(std::span<const uint64_t, N> a) {
   const auto max_bitlen = N*sizeof(uint64_t)*8;
   int leading_sign_bits = 0;
   for (int i = N-1; i > 0; --i) {
      auto sign_bits_in_limb = __builtin_clrsbll(a[i]) + 1;
      leading_sign_bits += sign_bits_in_limb;
      if (sign_bits_in_limb != 64)
         return max_bitlen - leading_sign_bits;
   }
   auto sign_bits_in_limb = __builtin_clrsbll(a[0]) + ((int64_t)a[0] >= 0);
   leading_sign_bits += sign_bits_in_limb;
   return max_bitlen - leading_sign_bits;
}

static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0, 0x0FFFFFFFFFFFFFFF}) == 252);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0x0FFFFFFFFFFFFFFF, 0}) == 188);
static_assert(bitlen<4>(array<uint64_t,4>{0x0FFFFFFFFFFFFFFF, 0, 0, 0}) == 60);

template <std::size_t N>
constexpr bool bit_test(std::span<const uint64_t, N> value, int i) {
   int limb_index    = i / (sizeof(uint64_t)*8);
   int index_in_limb = i % (sizeof(uint64_t)*8);
   return value[limb_index] & (1ULL << index_in_limb);
}
}