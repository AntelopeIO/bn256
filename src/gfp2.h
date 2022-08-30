#ifndef BN256_GFP2_H
#define BN256_GFP2_H

#include <gfp.h>

namespace bn256 {

    // gfp2 implements a field of size p² as a quadratic extension of the base field
    // where i²=-1.
    struct gfp2 {
        gfp x;
        gfp y;
        
        const gfp2& set(gfp2 &a) {
            gfp_set(x, a.x);
            gfp_set(y, a.y);
            return *this;
        }
        const gfp2& set_zero() {
            gfp_setzero(x);
            gfp_setzero(y);
            return *this;
        }
    };

}

#endif //BN256_GFP2_H
