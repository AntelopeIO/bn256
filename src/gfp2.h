#pragma once
#include <gfp.h>
#include <gfp_generic.h>

namespace bn256 {

// gfp2 implements a field of size p² as a quadratic extension of the base field
// where i²=-1.
struct gfp2 {
   gfp x_;
   gfp y_;

   static gfp2 gfp2_decode(const gfp2& in) noexcept;

   static constexpr gfp2 zero() noexcept { return {}; }
   static constexpr gfp2 one() noexcept { return {{}, gfp::one()}; }

   void set_zero() noexcept { *this = zero(); }
   void set_one() noexcept { *this = one(); }

   bool is_zero() const noexcept { return *this == zero(); }
   bool is_one() const noexcept { return *this == one(); }

   gfp2 conjugate() const noexcept;

   gfp2 neg() const noexcept;

   gfp2 add(const gfp2& b) const noexcept;

   gfp2 sub(const gfp2& b) const noexcept;

   gfp2 mul(const gfp2& b) const noexcept;

   gfp2 mul_scalar(const gfp& b) const noexcept;

   gfp2 mul_xi() const noexcept;

   gfp2 square() const noexcept;

   gfp2 invert() const noexcept;

   bool operator==(const gfp2& rhs) const noexcept {
      static_assert(std::is_standard_layout_v<gfp2>);
      return memcmp(this, &rhs, sizeof(*this)) == 0;
   }

   bool operator!=(const gfp2& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};

std::ostream& operator<<(std::ostream& os, const gfp2& v);

} // namespace bn256
