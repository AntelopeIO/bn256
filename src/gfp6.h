#pragma once
#include <gfp2.h>

namespace bn256 {

    // gfp6 implements the field of size p⁶ as a cubic extension of gfp2 where τ³=ξ
    // and ξ=i+9.
    struct gfp6 {
        // value is xτ² + yτ + z
        gfp2 x_;
        gfp2 y_;
        gfp2 z_;
        
        void set(const gfp6& a);

        const gfp6& set_zero();

        const gfp6& set_one();

        [[nodiscard]] bool is_zero() const ;

        [[nodiscard]] bool is_one() const ;

        const gfp6& neg(const gfp6& a);

        const gfp6& frobenius(const gfp6& a);

        const gfp6& frobenius_p2(const gfp6& a);

        const gfp6& frobenius_p4(const gfp6& a);

        const gfp6& add(const gfp6& a, const gfp6& b);

        const gfp6& sub(const gfp6& a, const gfp6& b);

        const gfp6& mul(const gfp6& a, const gfp6& b);

        const gfp6& mul_scalar(const gfp6& a, const gfp2& b);

        const gfp6& mul_gfp(const gfp6& a, const gfp& b);

        const gfp6& mul_tau(const gfp6& a);

        const gfp6& square(const gfp6& a);

        const gfp6& invert(const gfp6& a);

        bool operator==(const gfp6& rhs) const;

        bool operator!=(const gfp6& rhs) const;

    };

}
