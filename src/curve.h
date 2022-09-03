#ifndef BN256_CURVE_H
#define BN256_CURVE_H

#include <gfp.h>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::int256_t;
namespace bn256 {

    struct curve_point {
        // value is xτ² + yτ + z
        gfp x_;
        gfp y_;
        gfp z_;
        gfp t_;

        void set(const curve_point& a);
        bool is_on_curve();
        void set_infinity();
        [[nodiscard]] bool is_infinity() const;
        void add(const curve_point& a, const curve_point& b);
        void double_(const curve_point& point);
        void mul(const curve_point& a, const int256_t& scalar);
        void make_affine();
        void neg(const curve_point& a);
    };
}


#endif //BN256_CURVE_H
