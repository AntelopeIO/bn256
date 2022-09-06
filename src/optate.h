#ifndef BN256_OPTATE_H
#define BN256_OPTATE_H

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

    line_function_result_t line_function_add(const twist_point& r, const twist_point& p, const curve_point& q, const gfp2& r2);

    line_function_result_t line_function_double(twist_point& r, curve_point& q);

    void mul_line(gfp12& ret, gfp2& a, gfp2& b, gfp2& c);

    gfp12 miller(twist_point& q, curve_point& p);

    gfp12 final_exponentiation(gfp12& in);

    gfp12 optimal_ate(twist_point& q, curve_point& p);
}

#endif //BN256_OPTATE_H
