#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <gfp2.h>

using boost::multiprecision::int512_t;

namespace bn256 {

// twistPoint implements the elliptic curve y²=x³+3/ξ over GF(p²). Points are
// kept in Jacobian form and t=z² when valid. The group G₂ is the set of
// n-torsion points of this curve over GF(p²) (where n = Order)
struct twist_point {
   gfp2 x_;
   gfp2 y_;
   gfp2 z_;
   gfp2 t_;

   std::string string() const;

   // IsOnCurve returns true iff c is on the curve.
   bool is_on_curve() const noexcept;

   static constexpr twist_point infinity() noexcept { return { gfp2::zero(), gfp2::one(), gfp2::zero(), gfp2::zero() }; }

   [[nodiscard]] bool is_infinity() const noexcept { return z_ == gfp2::zero(); }

   twist_point add(const twist_point& b) const noexcept;

   twist_point double_() const noexcept;

   twist_point mul(const int512_t& scalar) const noexcept;

   twist_point make_affine() const noexcept;
   twist_point neg() const noexcept;

   bool operator==(const twist_point& rhs) const noexcept {
      static_assert(std::is_standard_layout_v<twist_point>);
      return memcmp(this, &rhs, sizeof(*this)) == 0;
   }

   bool operator!=(const twist_point& rhs) const noexcept { return !(*this == rhs); }
};


inline std::ostream& operator<<(std::ostream& os, const twist_point& v) { return os << v.string(); }
} // namespace bn256
