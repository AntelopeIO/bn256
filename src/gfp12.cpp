#include <gfp12.h>
#include <constants.h>
#include <bitlen.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <sstream>

namespace bn256 {

    const gfp12& gfp12::set(const gfp12& a) {
        x_.set(a.x_);
        y_.set(a.y_);
        return *this;
    }

    const gfp12& gfp12::set_zero() {
        x_.set_zero();
        y_.set_zero();
        return *this;
    }

    const gfp12& gfp12::set_one() {
        x_.set_zero();
        y_.set_one();
        return *this;
    }

    bool gfp12::is_zero() const {
        return x_.is_zero() &&
               y_.is_zero();
    }

    bool gfp12::is_one() const {
        return x_.is_zero() &&
               y_.is_one();
    }

    const gfp12& gfp12::conjugate(const gfp12& a) {
        x_.neg(a.x_);
        y_.set(a.y_);
        return *this;
    }

    const gfp12& gfp12::neg(const gfp12& a) {
        x_.neg(a.x_);
        y_.neg(a.y_);
        return *this;
    }

    // Frobenius computes (xω+y)^p = x^p ω·ξ^((p-1)/6) + y^p
    const gfp12& gfp12::frobenius(const gfp12& a) {
        x_.frobenius(a.x_);
        y_.frobenius(a.y_);
        x_.mul_scalar(x_, constants::xi_to_p_minus_1_over_6);
        return *this;
    }

    // FrobeniusP2 computes (xω+y)^p² = x^p² ω·ξ^((p²-1)/6) + y^p²
    const gfp12& gfp12::frobenius_p2(const gfp12& a) {
        x_.frobenius_p2(a.x_);
        x_.mul_gfp(x_, constants::xi_to_p_squared_minus_1_over_6);
        y_.frobenius_p2(a.y_);
        return *this;
    }

    const gfp12& gfp12::frobenius_p4(const gfp12& a) {
        x_.frobenius_p4(a.x_);
        x_.mul_gfp(x_, constants::xi_to_p_squared_minus_1_over_3);
        y_.frobenius_p4(a.y_);
        return *this;
    }

    const gfp12& gfp12::add(const gfp12& a, const gfp12& b) {
        x_.add(a.x_, b.x_);
        y_.add(a.y_, b.y_);
        return *this;
    }

    const gfp12& gfp12::sub(const gfp12& a, const gfp12& b) {
        x_.sub(a.x_, b.x_);
        y_.sub(a.y_, b.y_);
        return *this;
    }

    const gfp12& gfp12::mul(const gfp12& a, const gfp12& b) {
        gfp6 tx{}, t{}, ty{};

        tx.mul(a.x_, b.y_);
        t.mul(b.x_, a.y_);
        tx.add(tx, t);

        ty.mul(a.y_, b.y_);
        t.mul(a.x_, b.x_);
        t.mul_tau(t);

        x_.set(tx);
        y_.add(ty, t);
        return *this;
    }

    const gfp12& gfp12::mul_scalar(const gfp12& a, const gfp6& b) {
        x_.mul(x_, b);
        y_.mul(y_, b);
        return *this;
    }

    const gfp12& gfp12::exp(const gfp12& a, const int512_t& power) {
        gfp12 sum{}, t{};
        sum.set_one();

        for (int i = bitlen(power); i >= 0; i--) {
            t.square(sum);
            if (bit_test(power, i) != 0) {
                sum.mul(t, a);
            } else {
                sum.set(t);
            }
        }

        set(sum);
        return *this;
    }

    const gfp12& gfp12::square(const gfp12& a) {
        // Complex squaring algorithm
        gfp6 v0{}, t{}, ty{};

        v0.mul(a.x_, a.y_);

        t.mul_tau(a.x_);
        t.add(a.y_, t);
        ty.add(a.x_, a.y_);
        ty.mul(ty, t);
        ty.sub(ty, v0);

        t.mul_tau(v0);
        ty.sub(ty, t);

        x_.add(v0, v0);
        y_.set(ty);
        return *this;
    }

    const gfp12& gfp12::invert(const gfp12& a) {
        // See "Implementing cryptographic pairings", M. Scott, section 3.2.
        // ftp://136.206.11.249/pub/crypto/pairings.pdf
        gfp6 t1{}, t2{};

        t1.square(a.x_);
        t2.square(a.y_);
        t1.mul_tau(t1);
        t1.sub(t2, t1);

        t2.invert(t1);

        x_.neg(a.x_);
        y_.set(a.y_);
        mul_scalar(*this, t2);
        return *this;
    }

    bool gfp12::operator==(const gfp12& rhs) const {
        return x_ == rhs.x_ &&
               y_ == rhs.y_;
    }

    bool gfp12::operator!=(const gfp12& rhs) const {
        return !(rhs == *this);
    }

    std::string gfp12::string() {
        std::stringstream ss;
        ss << "(" << x_.string() << ", " << y_.string() << ")";
        return ss.str();
    }

}
