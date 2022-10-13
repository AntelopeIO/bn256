#pragma once

#include "twist.h"
#include "curve.h"
#include "gfp12.h"

namespace bn256 {

   gfp12 miller(const twist_point& q, const curve_point& p) noexcept;

   gfp12 final_exponentiation(const gfp12& in) noexcept;

   gfp12 optimal_ate(const twist_point& q, const curve_point& p) noexcept;
}
