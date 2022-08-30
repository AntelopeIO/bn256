#ifndef BN256_GFP_H
#define BN256_GFP_H

#include <cstdint>
#include <array>

namespace bn256 {

    typedef std::array<uint64_t, 4> uint64_array_4_t;
    typedef uint64_array_4_t gfp;

    static void gfp_set(gfp& e, gfp& f) {
        e = f;
    }

    static void gfp_setzero(gfp& e) {
        e.fill(0);
    }

}

#endif //BN256_GFP_H
