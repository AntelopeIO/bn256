#ifndef BN256_GFP_H
#define BN256_GFP_H

#include <cstdint>
#include <array>

namespace bn256 {
    
    typedef std::array<uint64_t, 4> uint64_array_4_t;
    typedef std::array<uint64_t, 8> uint64_array_8_t;
    typedef std::array<uint64_t, 18> uint64_array_18_t;
    typedef std::array<uint64_t, 32> uint64_array_32_t;
    typedef uint64_array_4_t gfp;

    static void gfp_set(gfp& e, gfp& f) {
        e = f;
    }

    static void gfp_setzero(gfp& e) {
        e.fill(0);
    }

    // gfp2 implements a field of size p² as a quadratic extension of the base field
    // where i²=-1.
    struct gfp2 {
        gfp x;
        gfp y;
        auto set(gfp2 &a) {
            gfp_set(x, a.x);
            gfp_set(y, a.y);
            return this;
        }
        auto set_zero() {
            gfp_setzero(x);
            gfp_setzero(y);
            return this;
        }
    };

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
        auto set_zero() {
            x.set_zero();
            y.set_zero();
            z.set_zero();
            return this;
        }
    } ;

    // gfp12 implements the field of size p¹² as a quadratic extension of gfp6
    // where ω²=τ.
    struct gfp12{
        // value is xω + y
        gfp6 x;
        gfp6 y;
        auto set(gfp12 &a) {
            x.set(a.x);
            y.set(a.y);
            return this;
        }
        auto set_zero(gfp12 &e) {
            x.set_zero();
            y.set_zero();
            return this;
        }
    };

    void gfp_carry(gfp& a, uint64_t head);
    void gfp_neg(gfp& c, const gfp& a);
    void gfp_add(gfp& c, const gfp& a, const gfp& b);
    void gfp_sub(gfp& c, const gfp& a, const gfp& b);
    void gfp_mul(gfp& c, const gfp& a, const gfp& b);
}

#endif //BN256_GFP_H
