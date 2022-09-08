#include <gfp2.h>

namespace bn256 {

    gfp2 gfp2::gfp2_decode(const gfp2& in) {
        gfp2 out{};
        out.x_.mont_decode(in.x_);
        out.y_.mont_decode(in.y_);
        return out;
    }

    const gfp2& gfp2::set(const gfp2& a) {
        x_.set(a.x_);
        y_.set(a.y_);
        return *this;
    }

    const gfp2& gfp2::set_zero() {
        x_.set_zero();
        y_.set_zero();
        return *this;
    }

    const gfp2& gfp2::set_one() {
        x_.set_zero();
        y_ = {1};
        return *this;
    }

    bool gfp2::is_zero() const {
        constexpr gfp zero{};
        return zero == x_ && zero == y_;
    }

    bool gfp2::is_one() const {
        constexpr gfp zero{};
        constexpr gfp one{1};
        return zero == x_ && one == y_;
    }

    const gfp2& gfp2::conjugate(const gfp2& a) {
        y_.set(a.y_);
        gfp_neg(x_, a.x_);
        return *this;
    }

    const gfp2& gfp2::neg(const gfp2& a) {
        gfp_neg(x_,a.x_);
        gfp_neg(y_,a.y_);
        return *this;
    }

    const gfp2& gfp2::add(const gfp2& a, const gfp2& b) {
        gfp_add(x_, a.x_, b.x_);
        gfp_add(y_, a.y_, b.y_);
        return *this;
    }

    const gfp2& gfp2::sub(const gfp2& a, const gfp2& b) {
        gfp_sub(x_, a.x_, b.x_);
        gfp_sub(y_, a.y_, b.y_);
        return *this;
    }

    // See "Multiplication and Squaring in Pairing-Friendly Fields",
    // http://eprint.iacr.org/2006/471.pdf
    const gfp2& gfp2::mul(const gfp2& a, const gfp2& b) {
        gfp tx{}, t{}, ty{};

        gfp_mul(tx, a.x_, b.y_);
        gfp_mul(t, b.x_, a.y_);
        gfp_add(tx,tx, t);

        gfp_mul(ty, a.y_, b.y_);
        gfp_mul(t, a.x_, b.x_);
        gfp_sub(ty, ty, t);

        x_.set(tx);
        y_.set(ty);

        return *this;
    }

    const gfp2& gfp2::mul_scalar(const gfp2& a, const gfp& b) {
        gfp_mul(x_, a.x_, b);
        gfp_mul(y_, a.y_, b);
        return *this;
    }

    // MulXi sets e=ξa where ξ=i+9 and then returns e.
    const gfp2& gfp2::mul_xi(const gfp2& a) {
        gfp tx{}, ty{};
        gfp_add(tx, a.x_, a.x_);
        gfp_add(tx, tx, tx);
        gfp_add(tx, tx, tx);
        gfp_add(tx, tx, a.x_);

        gfp_add(tx, tx, a.y_);

        gfp_add(ty, a.y_, a.y_);
        gfp_add(ty, ty, ty);
        gfp_add(ty, ty, ty);
        gfp_add(ty, ty, a.y_);

        gfp_add(ty, ty, a.x_);

        x_.set(tx);
        y_.set(ty);

        return *this;
    }

    const gfp2& gfp2::square(const gfp2& a) {
        // Complex squaring algorithm:
        // (xi+y)² = (x+y)(y-x) + 2*i*x*y
        gfp tx{}, ty{};
        gfp_sub(tx, a.y_, a.x_);
        gfp_add(ty, a.x_, a.y_);
        gfp_mul(ty, tx, ty);

        gfp_mul(tx, a.x_, a.y_);
        gfp_add(tx, tx, tx);

        x_.set(tx);
        y_.set(ty);

        return *this;
    }

    const gfp2& gfp2::invert(const gfp2& a) {
        // See "Implementing cryptographic pairings", M. Scott, section 3.2.
        // ftp://136.206.11.249/pub/crypto/pairings.pdf
        gfp t1{}, t2{}, inv{};

        gfp_mul(t1, a.x_, a.x_);
        gfp_mul(t2, a.y_, a.y_);
        gfp_add(t1, t1, t2);

        inv.invert(t1);

        gfp_neg(t1, a.x_);

        gfp_mul(x_, t1, inv);
        gfp_mul(y_, a.y_, inv);

        return *this;
    }

    bool gfp2::operator==(const gfp2& rhs) const {
        return x_ == rhs.x_ &&
               y_ == rhs.y_;
    }

    bool gfp2::operator!=(const gfp2& rhs) const {
        return !(rhs == *this);
    }

}
