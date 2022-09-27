#pragma once

#include <twist.h>
#include <curve.h>
#include <gfp2.h>
#include <gfp12.h>

namespace bn256 {

   struct line_function_result_t {
      gfp2 a_{};
      gfp2 b_{};
      gfp2 c_{};
      twist_point out_{};
   };

   line_function_result_t line_function_add(
      const twist_point& r,
      const twist_point& p,
      const curve_point& q,
      const gfp2& r2);

   line_function_result_t line_function_double(
      const twist_point& r,
      const curve_point& q);

   void mul_line(
      gfp12& ret,
      const gfp2& a,
      const gfp2& b,
      const gfp2& c);

   gfp12 miller(const twist_point& q, const curve_point& p);

   gfp12 final_exponentiation(const gfp12& in);

   gfp12 optimal_ate(const twist_point& q, const curve_point& p);
}
