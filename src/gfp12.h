#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <gfp6.h>

using boost::multiprecision::int512_t;

namespace bn256 {

// gfp12 implements the field of size p¹² as a quadratic extension of gfp6
// where ω²=τ.
struct gfp12 {
   // value is xω + y
   gfp6 x_;
   gfp6 y_;

   static constexpr gfp12 zero() noexcept { return {}; }
   static constexpr gfp12 one() noexcept { return {{}, gfp6::one()}; }

   void set_zero() noexcept { *this = zero(); }
   void set_one() noexcept { *this = one(); }

   bool is_zero() const noexcept { return *this == zero(); }
   bool is_one() const noexcept { return *this == one(); }

   gfp12 conjugate() const noexcept;

   gfp12 neg() const noexcept;

   gfp12 frobenius() const noexcept;

   gfp12 frobenius_p2() const noexcept;

   gfp12 frobenius_p4() const noexcept;

   gfp12 add(const gfp12& b) const noexcept;

   gfp12 sub(const gfp12& b) const noexcept;

   gfp12 mul(const gfp12& b) const noexcept;

   gfp12 mul_scalar(const gfp6& b) const noexcept;

   gfp12 exp(const int512_t& power) const noexcept;

   gfp12 square() const noexcept;

   gfp12 invert() const noexcept;

   bool operator==(const gfp12& rhs) const noexcept {
      static_assert(std::is_standard_layout_v<gfp12>);
      return memcmp(this, &rhs, sizeof(*this)) == 0;
   }

   bool operator!=(const gfp12& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};

std::ostream& operator<<(std::ostream& os, const gfp12& v);

} // namespace bn256
