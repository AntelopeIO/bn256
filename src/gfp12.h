#pragma once

#include <gfp6.h>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::int512_t;

namespace bn256 {

   // gfp12 implements the field of size p¹² as a quadratic extension of gfp6
   // where ω²=τ.
   struct gfp12 {
      // value is xω + y
      gfp6 x_;
      gfp6 y_;

      const gfp12& set(const gfp12& a);

      const gfp12& set_zero();

      const gfp12& set_one();

      [[nodiscard]] bool is_zero() const;

      [[nodiscard]] bool is_one() const;

      const gfp12& conjugate(const gfp12& a);

      const gfp12& neg(const gfp12& a);

      const gfp12& frobenius(const gfp12& a);

      const gfp12& frobenius_p2(const gfp12& a);

      const gfp12& frobenius_p4(const gfp12& a);

      const gfp12& add(const gfp12& a, const gfp12& b);

      const gfp12& sub(const gfp12& a, const gfp12& b);

      const gfp12& mul(const gfp12& a, const gfp12& b);

      const gfp12& mul_scalar(const gfp12& a, const gfp6& b);

      const gfp12& exp(const gfp12& a, const int512_t& power);

      const gfp12& square(const gfp12& a);

      const gfp12& invert(const gfp12& a);

      bool operator==(const gfp12& rhs) const;

      bool operator!=(const gfp12& rhs) const;

      std::string string();

   };

   std::ostream& operator << (std::ostream& os, const gfp12& v);

}
