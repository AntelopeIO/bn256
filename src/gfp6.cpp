#include <gfp6.h>
#include <constants.h>

namespace bn256 {

    void gfp6::set(const gfp6& a) {
        x_.set(a.x_);
        y_.set(a.y_);
        z_.set(a.z_);
    }

    const gfp6& gfp6::set_zero() {
        x_.set_zero();
        y_.set_zero();
        z_.set_zero();
        return *this;
    }

    const gfp6& gfp6::set_one() {
        x_.set_zero();
        y_.set_zero();
        z_.set_one();
        return *this;
    }

    bool gfp6::is_zero() const {
        return x_.is_zero() &&
               y_.is_zero() &&
               z_.is_zero();
    }

    bool gfp6::is_one() const {
        return x_.is_zero() &&
               y_.is_zero() &&
               z_.is_one();
    }

    const gfp6& gfp6::neg(const gfp6& a) {
        x_.neg(a.x_);
        return *this;
    }

    const gfp6& gfp6::frobenius(const gfp6& a) {
        x_.conjugate(a.x_);
        y_.conjugate(a.y_);
        z_.conjugate(a.z_);

        x_.mul(x_, constants::xi_to_2p_minus_2_over_3);
        y_.mul(y_, constants::xi_to_p_minus_1_over_3);
        return *this;
    }

    // FrobeniusP2 computes (xτ²+yτ+z)^(p²) = xτ^(2p²) + yτ^(p²) + z
    const gfp6& gfp6::frobenius_p2(const gfp6& a) {
        // τ^(2p²) = τ²τ^(2p²-2) = τ²ξ^((2p²-2)/3)
        x_.mul_scalar(a.x_, constants::xi_to_2p_squared_minus_2_over_3);
        // τ^(p²) = ττ^(p²-1) = τξ^((p²-1)/3)
        y_.mul_scalar(a.y_, constants::xi_to_p_squared_minus_1_over_3);
        z_.set(a.z_);
        return *this;
    }

    const gfp6& gfp6::frobenius_p4(const gfp6& a) {
        x_.mul_scalar(a.x_, constants::xi_to_p_squared_minus_1_over_3);
        y_.mul_scalar(a.y_, constants::xi_to_2p_squared_minus_2_over_3);
        z_.set(a.z_);
        return *this;
    }

    const gfp6& gfp6::add(const gfp6& a, const gfp6& b) {
        x_.add(a.x_, b.x_);
        y_.add(a.y_, b.y_);
        z_.add(a.z_, b.z_);
        return *this;
    }

    const gfp6& gfp6::sub(const gfp6& a, const gfp6& b) {
        x_.sub(a.x_, b.x_);
        y_.sub(a.y_, b.y_);
        z_.sub(a.z_, b.z_);
        return *this;
    }

    const gfp6& gfp6::mul(const gfp6& a, const gfp6& b) {
        // "Multiplication and Squaring on Pairing-Friendly Fields"
        // Section 4, Karatsuba method.
        // http://eprint.iacr.org/2006/471.pdf
        gfp2 v0{}, v1{}, v2{}, t0{}, t1{}, tz{}, ty{}, tx{};

        v0.mul(a.z_, b.z_);
        v1.mul(a.y_, b.y_);
        v2.mul(a.x_, b.x_);

        t0.add(a.x_, a.y_);
        t1.add(b.x_, b.y_);
        tz.mul(t0, t1);
        tz.sub(tz, v1);
        tz.sub(tz, v2);
        tz.mul_xi(tz);
        tz.add(tz, v0);

        t0.add(a.y_, a.z_);
        t1.add(b.y_, b.z_);
        ty.mul(t0, t1);
        t0.mul_xi(v2);
        ty.sub(ty, v0);
        ty.sub(ty, v1);
        ty.add(ty, t0);

        t0.add(a.x_, a.z_);
        t1.add(b.x_, b.z_);
        tx.mul(t0, t1);
        tx.sub(tx, v0);
        tx.add(tx, v1);
        tx.sub(tx, v2);

        x_.set(tx);
        y_.set(ty);
        z_.set(tz);

        return *this;
    }

    const gfp6& gfp6::mul_scalar(const gfp6& a, const gfp2& b) {
        x_.mul(a.x_, b);
        y_.mul(a.y_, b);
        z_.mul(a.z_, b);
        return *this;
    }

    const gfp6& gfp6::mul_gfp(const gfp6& a, const gfp& b) {
        x_.mul_scalar(a.x_, b);
        y_.mul_scalar(a.y_, b);
        z_.mul_scalar(a.z_, b);
        return *this;
    }

    const gfp6& gfp6::mul_tau(const gfp6& a) {
        gfp2 tz{}, ty{};

        tz.mul_xi(a.x_);
        ty.set(a.y_);

        y_.set(a.z_);
        x_.set(ty);
        z_.set(tz);
        return *this;
    }

    const gfp6& gfp6::square(const gfp6& a) {
        gfp2 v0{}, v1{}, v2{}, c0{}, c1{}, xi_v2{}, c2{};

        v0.square(a.z_);
        v1.square(a.y_);
        v2.square(a.x_);

        c0.add(a.x_, a.y_);
        c0.square(c0);
        c0.sub(c0, v1);
        c0.sub(c0, v2);
        c0.mul_xi(c0);
        c0.add(c0, v0);

        c1.add(a.y_, a.z_);
        c1.square(c1);
        c1.sub(c1, v0);
        c1.sub(c1, v1);

        xi_v2.mul_xi(v2);
        c1.add(c1, xi_v2);

        c2.add(a.x_, a.z_);
        c2.square(c2);
        c2.sub(c2, v0);
        c2.add(c2, v1);
        c2.sub(c2, v2);

        x_.set(c2);
        y_.set(c1);
        z_.set(c0);
        return *this;
    }

    const gfp6& gfp6::invert(const gfp6& a) {
        // See "Implementing cryptographic pairings", M. Scott, section 3.2.
        // ftp://136.206.11.249/pub/crypto/pairings.pdf

        // Here we can give a short explanation of how it works: let j be a cubic root of
        // unity in GF(p²) so that 1+j+j²=0.
        // Then (xτ² + yτ + z)(xj²τ² + yjτ + z)(xjτ² + yj²τ + z)
        // = (xτ² + yτ + z)(Cτ²+Bτ+A)
        // = (x³ξ²+y³ξ+z³-3ξxyz) = F is an element of the base field (the norm).
        //
        // On the other hand (xj²τ² + yjτ + z)(xjτ² + yj²τ + z)
        // = τ²(y²-ξxz) + τ(ξx²-yz) + (z²-ξxy)
        //
        // So that's why A = (z²-ξxy), B = (ξx²-yz), C = (y²-ξxz)
        gfp2 t1{}, a2{}, b{}, c{}, f{};

        t1.mul(a.x_, a.y_);
        t1.mul_xi(t1);

        a2.square(a.z_);
        a2.sub(a2, t1);

        b.square(a.x_);
        b.mul_xi(b);
        t1.mul(a.y_, a.z_);
        b.sub(b, t1);

        c.square(a.y_);
        t1.mul(a.x_, a.z_);
        c.sub(c, t1);

        f.mul(c, a.y_);

        f.mul_xi(f);
        t1.mul(a2, a.z_);
        f.add(f, t1);
        t1.mul(b, a.x_);
        t1.mul_xi(t1);

        f.add(f, t1);

        f.invert(f);

        x_.mul(c, f);
        y_.mul(b, f);
        z_.mul(a2, f);
        return *this;
    }

}
