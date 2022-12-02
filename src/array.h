#pragma once
#include <cstddef>
#include <cstring>
#if __cplusplus > 201703L
#   include <array>
namespace bn256 {
template <typename T, std::size_t S>
using array = std::array<T, S>;
}
#else
#include <bn256/span.h>

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
   auto bit_length = N*sizeof(uint64_t)*8;
   const uint64_t all_sign_bits = static_cast<int64_t>(a[N-1]) >> 63;

   int i = N-1;
   while (i >0 && a[i] == all_sign_bits) {
        bit_length -= sizeof(uint64_t)*8;
        --i;
   }
   if (a[i]==0 ) return 0;
   return bit_length - __builtin_clzll(all_sign_bits == 0 ? a[i] : -a[i]);
}

static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0, 0x0FFFFFFFFFFFFFFF}) == 252);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0x0FFFFFFFFFFFFFFF, 0}) == 188);
static_assert(bitlen<4>(array<uint64_t,4>{0x0FFFFFFFFFFFFFFF, 0, 0, 0}) == 60);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0, 0x8FFFFFFFFFFFFFFF}) == 255);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0, 0x7FFFFFFFFFFFFFFF}) == 255);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}) == 192);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0x7FFFFFFFFFFFFFFF, 0}) == 191);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0x8FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}) == 191);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0x8FFFFFFFFFFFFFFF, 0}) == 192);
static_assert(bitlen<4>(array<uint64_t,4>{0x7FFFFFFFFFFFFFFF, 0, 0, 0}) == 63);
static_assert(bitlen<4>(array<uint64_t,4>{0x8FFFFFFFFFFFFFFF, 0, 0, 0}) == 64);
static_assert(bitlen<4>(array<uint64_t,4>{0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}) == 64);
static_assert(bitlen<4>(array<uint64_t,4>{0x8FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}) == 63);
static_assert(bitlen<4>(array<uint64_t,4>{0, 0, 0, 0}) == 0);

constexpr int bitlen(const array<uint64_t, 4>& a) {
   return bitlen<4>(std::span<const uint64_t, 4>(a));
}

template <std::size_t N>
constexpr bool bit_test(std::span<const uint64_t, N> value, int i) {
   int limb_index    = i / (sizeof(uint64_t)*8);
   int index_in_limb = i % (sizeof(uint64_t)*8);
   return value[limb_index] & (1ULL << index_in_limb);
}
}