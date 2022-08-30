#ifndef BN256_GFP12_H
#define BN256_GFP12_H

#include <gfp6.h>

namespace bn256 {

    // gfp12 implements the field of size p¹² as a quadratic extension of gfp6
    // where ω²=τ.
    struct gfp12 {
        // value is xω + y
        gfp6 x;
        gfp6 y;

        const gfp12& set(gfp12& a) {
            x.set(a.x);
            y.set(a.y);
            return *this;
        }
        const gfp12& set_zero() {
            x.set_zero();
            y.set_zero();
            return *this;
        }
    };

}

#endif //BN256_GFP12_H
