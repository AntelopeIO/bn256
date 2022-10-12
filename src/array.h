#pragma once

#if __cplusplus > 201703L
#   include <array>
namespace bn256 {
template <typename T, std::size_t S>
using array = std::array<T, S>;
}
#else

namespace bn256 {
// std::array in C++17 cannot be used in constexpr context; therefore we rollout our own.
template <typename T, std::size_t N>
struct array {
   T                     v_[N];
   constexpr T&          operator[](std::size_t i) noexcept { return v_[i]; }
   constexpr const T&    operator[](std::size_t i) const noexcept { return v_[i]; }
   constexpr std::size_t size() const noexcept { return N; }

   constexpr bool operator==(const array<uint64_t, N>& other) const noexcept {
      for (std::size_t i = 0; i < N; ++i)
         if (v_[i] != other[i])
            return false;
      return true;
   }

   constexpr bool operator!=(const array& other) const noexcept { return !(*this == other); }
};
} // namespace bn256
#endif