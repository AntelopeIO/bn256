#pragma once
#include <gfp.h>
#include <gfp_generic.h>

namespace bn256 {

    // gfp2 implements a field of size p² as a quadratic extension of the base field
    // where i²=-1.
    struct gfp2 {
        gfp x_{};
        gfp y_{};

        static gfp2 gfp2_decode(const gfp2& in);

        const gfp2& set(const gfp2& a);

        const gfp2& set_zero();

        const gfp2& set_one();

        [[nodiscard]] bool is_zero() const;

        [[nodiscard]] bool is_one() const;

        const gfp2& conjugate(const gfp2& a);

        const gfp2& neg(const gfp2& a);

        const gfp2& add(const gfp2& a, const gfp2& b);

        const gfp2& sub(const gfp2& a, const gfp2& b);

        const gfp2& mul(const gfp2& a, const gfp2& b);

        const gfp2& mul_scalar(const gfp2& a, const gfp& b);

        const gfp2& mul_xi(const gfp2& a);

        const gfp2& square(const gfp2& a);

        const gfp2& invert(const gfp2& a);

        bool operator==(const gfp2& rhs) const;

        bool operator!=(const gfp2& rhs) const;
    };

}
