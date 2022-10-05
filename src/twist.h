#pragma once

#include <gfp2.h>
#include <boost/multiprecision/cpp_int.hpp>

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

      std::string string() const ;

      void set(const twist_point& a);

      // IsOnCurve returns true iff c is on the curve.
      bool is_on_curve();

      void set_infinity();

      [[nodiscard]] bool is_infinity() const;

      void add(const twist_point& a, const twist_point& b);

      void double_(const twist_point& a);

      void mul(const twist_point& a, const int512_t& scalar);

      twist_point make_affine() const;

      void neg(const twist_point& a);
   };

   inline std::ostream& operator << (std::ostream& os, const twist_point& v) {
      return os << v.string();
   }
}
