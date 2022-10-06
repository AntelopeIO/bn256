#pragma once

#include <gfp.h>
#include <boost/multiprecision/cpp_int.hpp>

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

      void set(const curve_point& a);

      // IsOnCurve returns true iff c is on the curve.
      bool is_on_curve() const;

      void set_infinity();

      [[nodiscard]] bool is_infinity() const;

      void add(const curve_point& a, const curve_point& b);

      void double_(const curve_point& point);

      void mul(const curve_point& a, const int512_t& scalar);

      void make_affine();

      void neg(const curve_point& a);
   };
}
