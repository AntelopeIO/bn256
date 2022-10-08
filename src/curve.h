#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <gfp.h>

using boost::multiprecision::int512_t;

namespace bn256 {

// curvePoint implements the elliptic curve y²=x³+3. Points are kept in Jacobian
// form and t=z² when valid. G₁ is the set of points of this curve on GF(p).
struct curve_point {
   // value is xτ² + yτ + z
   gfp x_;
   gfp y_;
   gfp z_;
   gfp t_;

   std::string string() const;
   // IsOnCurve returns true iff c is on the curve.
   bool is_on_curve() const noexcept;

   static const curve_point infinity;
   bool                     is_infinity() const noexcept { return z_ == gfp{ 0 }; }

   curve_point add(const curve_point& b) const noexcept;

   curve_point double_() const noexcept;

   curve_point mul(const int512_t& scalar) const noexcept;

   [[nodiscard]] curve_point make_affine() const noexcept;

   curve_point neg() const noexcept;

   bool operator==(const curve_point& rhs) const noexcept {
      static_assert(std::is_standard_layout_v<curve_point>);
      return memcmp(this, &rhs, sizeof(*this)) == 0;
   }

   bool operator!=(const curve_point& rhs) const noexcept { return !(*this == rhs); }
};

inline const curve_point curve_point::infinity = { { 0 }, new_gfp(1), { 0 }, { 0 } };

inline std::ostream& operator<<(std::ostream& os, const curve_point& v) { return os << v.string(); }
} // namespace bn256
