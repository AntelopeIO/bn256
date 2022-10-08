#pragma once

#include <gfp2.h>

namespace bn256 {

// gfp6 implements the field of size p⁶ as a cubic extension of gfp2 where τ³=ξ
// and ξ=i+9.
struct gfp6 {
   // value is xτ² + yτ + z
   gfp2 x_;
   gfp2 y_;
   gfp2 z_;

   static constexpr gfp6 zero() noexcept { return {}; }
   static constexpr gfp6 one()  noexcept { return { {}, {}, gfp2::one()}; }

   [[nodiscard]] bool is_zero() const noexcept { return *this == zero(); }
   [[nodiscard]] bool is_one() const noexcept { return *this == one(); }

   gfp6 neg() const noexcept;

   gfp6 frobenius() const noexcept;

   gfp6 frobenius_p2() const noexcept;

   gfp6 frobenius_p4() const noexcept;

   gfp6 add(const gfp6& b) const noexcept;

   gfp6 sub(const gfp6& b) const noexcept;

   gfp6 mul(const gfp6& b) const noexcept;

   gfp6 mul_scalar(const gfp2& b) const noexcept;

   gfp6 mul_gfp(const gfp& b) const noexcept;

   gfp6 mul_tau() const noexcept;

   gfp6 square() const noexcept;

   gfp6 invert() const noexcept;

   bool operator==(const gfp6& rhs) const noexcept {
      static_assert(std::is_standard_layout_v<gfp6>);
      return memcmp(this, &rhs, sizeof(*this)) == 0;
   }

   bool operator!=(const gfp6& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};


std::ostream& operator<<(std::ostream& os, const gfp6& v);
} // namespace bn256
