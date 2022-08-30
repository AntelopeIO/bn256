#ifndef BN256_GFP6_H
#define BN256_GFP6_H

#include <gfp2.h>

namespace bn256 {

    // gfp6 implements the field of size p⁶ as a cubic extension of gfp2 where τ³=ξ
    // and ξ=i+9.
    struct gfp6 {
        // value is xτ² + yτ + z
        gfp2 x;
        gfp2 y;
        gfp2 z;
        
        void set(gfp6 &a) {
            x.set(a.x);
            y.set(a.y);
            y.set(a.z);
        }
        const gfp6& set_zero() {
            x.set_zero();
            y.set_zero();
            z.set_zero();
            return *this;
        }
    };

}

#endif //BN256_GFP6_H
