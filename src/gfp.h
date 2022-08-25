#ifndef BN256_GFP_H
#define BN256_GFP_H

#include <cstdint>
#include <array>

namespace bn256 {
    
    typedef std::array<uint64_t, 4> uint64_array_4_t;
    typedef std::array<uint64_t, 8> uint64_array_8_t;
    typedef std::array<uint64_t, 18> uint64_array_18_t;
    typedef std::array<uint64_t, 32> uint64_array_32_t;
    typedef uint64_array_4_t gfP;

    static void gfp_set(gfP& e, gfP& f) {
        e = f;
    }

    static void gfp_setzero(gfP& e) {
        e.fill(0);
    }

    // gfP2 implements a field of size p² as a quadratic extension of the base field
    // where i²=-1.
    struct gfP2 {
        gfP x;
        gfP y;
        auto set(gfP2 &a) {
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

    // gfP6 implements the field of size p⁶ as a cubic extension of gfP2 where τ³=ξ
    // and ξ=i+9.
    struct gfP6 {
        // value is xτ² + yτ + z
        gfP2 x;
        gfP2 y;
        gfP2 z;
        void set(gfP6 &a) {
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

    // gfP12 implements the field of size p¹² as a quadratic extension of gfP6
    // where ω²=τ.
    struct gfP12{
        // value is xω + y
        gfP6 x;
        gfP6 y;
        auto set(gfP12 &a) {
            x.set(a.x);
            y.set(a.y);
            return this;
        }
        auto set_zero(gfP12 &e) {
            x.set_zero();
            y.set_zero();
            return this;
        }
    };

    void gfp_carry(gfP& a, uint64_t head);
    void gfp_neg(gfP& c, const gfP& a);
    void gfp_add(gfP& c, const gfP& a, const gfP& b);
    void gfp_sub(gfP& c, const gfP& a, const gfP& b);
    void gfp_mul(gfP& c, const gfP& a, const gfP& b);
}

#endif //BN256_GFP_H
