#include <bn256.h>
#include <curve.h>
#include <optate.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <random_256.h>
#include <sstream>

namespace bn256 {
    using namespace boost::multiprecision::literals;
    using namespace boost::multiprecision;

    // scalar_base_mult sets g1 to g*k where g is the generator of the group and then
    // returns g1.
    const g1& g1::scalar_base_mult(const int512_t& k) {
        p_.mul(curve_gen, k);
        return *this;
    }

    // scalar_mult sets g1 to a*k and then returns g1.
    const g1& g1::scalar_mult(const g1& a, const int512_t& k) {
        p_.mul(a.p_, k);
        return *this;
    }

    // add sets g1 to a+b and then returns g1.
    const g1& g1::add(const g1& a, const g1& b) {
        p_.add(a.p_, b.p_);
        return *this;
    }

    // neg sets g1 to -a and then returns g1.
    const g1& g1::neg(const g1& a) {
        p_.neg(a.p_);
        return *this;
    }

    // set sets g1 to a and then returns g1.
    const g1& g1::set(const g1& a) {
        p_.set(a.p_);
        return *this;
    }

    // marshal converts g1 to a byte slice.
    [[nodiscard]] uint8_array_32_array_2_t g1::marshal() {

        uint8_array_32_array_2_t ret {};

        p_.make_affine();
        if (p_.is_infinity()) {
            return ret;
        }

        gfp temp{};
        temp.mont_decode(p_.x_);
        temp.marshal(ret[0]);

        temp.mont_decode(p_.y_);
        temp.marshal(ret[1]);

        return ret;
    }

    // unmarshal sets g1 to the result of converting the output of marshal back into
    // a group element and then returns unmarshal_status.
    unmarshal_status g1::unmarshal(uint8_array_32_array_2_t& m) {

        p_.x_.unmarshal(m[0]);
        p_.x_.mont_encode(p_.x_);

        p_.y_.unmarshal(m[1]);
        p_.y_.mont_encode(p_.y_);

        constexpr gfp zero {};
        if (p_.x_ == zero && p_.y_ == zero) {
            // This is the point at infinity.
            p_.y_ = new_gfp(1);
            p_.z_ = zero;
            p_.t_ = zero;
        } else {
            p_.z_ = new_gfp(1);
            p_.t_ = new_gfp(1);
            if (!p_.is_on_curve()) {
                return unmarshal_malformed_point;
            }
        }
        return unmarshal_success;
    }

    std::string g1::string() {
        std::stringstream ss;
        ss << "bn256.g1" << p_.string();
        return ss.str();
    }

    // scalar_base_mult sets g2 to g*k where g is the generator of the group and then
    // returns out.
    const g2& g2::scalar_base_mult(const int512_t& k) {
        p_.mul(twist_gen, k);
        return *this;
    }

    // scalar_mult sets g2 to a*k and then returns g2.
    const g2& g2::scalar_mult(const g2& a, const int512_t& k) {
        p_.mul(a.p_, k);
        return *this;
    }

    // add sets g2 to a+b and then returns g2.
    const g2& g2::add(const g2& a, const g2& b) {
        p_.add(a.p_, b.p_);
        return *this;
    }

    // neg sets g2 to -a and then returns g2.
    const g2& g2::neg(const g2& a) {
        p_.neg(a.p_);
        return *this;
    }

    // set sets g2 to a and then returns g2.
    const g2& g2::set(const g2& a) {
        p_.set(a.p_);
        return *this;
    }

    // marshal converts g2 to a byte slice.
    uint8_array_32_array_4_t g2::marshal() {
        uint8_array_32_array_4_t ret {};

        p_.make_affine();
        if (p_.is_infinity()) {
            return ret;
        }

        gfp temp{};

        temp.mont_decode(p_.x_.x_);
        temp.marshal(ret[0]);

        temp.mont_decode(p_.x_.y_);
        temp.marshal(ret[1]);

        temp.mont_decode(p_.y_.x_);
        temp.marshal(ret[2]);

        temp.mont_decode(p_.y_.y_);
        temp.marshal(ret[3]);

        return ret;
    }

    // unmarshal sets g2 to the result of converting the output of marshal back into
    // a group element and then returns unmarshal_status.
    unmarshal_status g2::unmarshal(uint8_array_32_array_4_t& m) {
        unmarshal_status status = p_.x_.x_.unmarshal(m[0]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.x_.y_.unmarshal(m[1]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.x_.unmarshal(m[2]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.y_.unmarshal(m[3]);
        if (status != unmarshal_success) {
            return status;
        }

        p_.x_.x_.mont_encode(p_.x_.x_);
        p_.x_.y_.mont_encode(p_.x_.y_);
        p_.y_.x_.mont_encode(p_.y_.x_);
        p_.y_.y_.mont_encode(p_.y_.y_);

        if (p_.x_.is_zero() && p_.y_.is_zero()) {
            // This is the point at infinity.
            p_.y_.set_one();
            p_.z_.set_zero();
            p_.t_.set_zero();
        } else {
            p_.z_.set_one();
            p_.t_.set_one();
            if (!p_.is_on_curve()) {
                return unmarshal_malformed_point;
            }
        }

        return status;
    }

    std::string g2::string() {
        std::stringstream ss;
        ss << "bn256.g2" << p_.string();
        return ss.str();
    }

    const gt& gt::scalar_mult(const gt& a, const int512_t& k) {
        p_.exp(a.p_, k);
        return *this;
    }

    const gt& gt::add(const gt& a, const gt& b) {
        p_.mul(a.p_, b.p_);
        return *this;
    }

    const gt& gt::neg(const gt& a) {
        p_.conjugate(a.p_);
        return *this;
    }

    const gt& gt::set(const gt& a) {
        p_.set(a.p_);
        return *this;
    }

    const gt& gt::set(const gfp12& a) {
        p_.set(a);
        return *this;
    }


    const gt& gt::finalize() {
        auto ret = final_exponentiation(p_);
        p_.set(ret);
        return *this;
    }

    uint8_array_32_array_12_t gt::marshal() const {
        uint8_array_32_array_12_t ret {};

        gfp temp{};

        temp.mont_decode(p_.x_.x_.x_);
        temp.marshal(ret[0]);

        temp.mont_decode(p_.x_.x_.y_);
        temp.marshal(ret[1]);

        temp.mont_decode(p_.x_.y_.x_);
        temp.marshal(ret[2]);

        temp.mont_decode(p_.x_.y_.y_);
        temp.marshal(ret[3]);

        temp.mont_decode(p_.x_.z_.x_);
        temp.marshal(ret[4]);

        temp.mont_decode(p_.x_.z_.y_);
        temp.marshal(ret[5]);

        temp.mont_decode(p_.y_.x_.x_);
        temp.marshal(ret[6]);

        temp.mont_decode(p_.y_.x_.y_);
        temp.marshal(ret[7]);

        temp.mont_decode(p_.y_.y_.x_);
        temp.marshal(ret[8]);

        temp.mont_decode(p_.y_.y_.y_);
        temp.marshal(ret[9]);

        temp.mont_decode(p_.y_.z_.x_);
        temp.marshal(ret[10]);

        temp.mont_decode(p_.y_.z_.y_);
        temp.marshal(ret[11]);

        return ret;
    }

    unmarshal_status  gt::unmarshal(uint8_array_32_array_12_t& m) {
        unmarshal_status status = p_.x_.x_.x_.unmarshal(m[0]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.x_.x_.y_.unmarshal(m[1]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.x_.y_.x_.unmarshal(m[2]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.x_.y_.y_.unmarshal(m[3]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.x_.z_.x_.unmarshal(m[4]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.x_.z_.y_.unmarshal(m[5]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.x_.x_.unmarshal(m[6]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.x_.y_.unmarshal(m[7]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.y_.x_.unmarshal(m[8]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.y_.y_.unmarshal(m[9]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.z_.x_.unmarshal(m[10]);
        if (status != unmarshal_success) {
            return status;
        }

        status = p_.y_.z_.y_.unmarshal(m[11]);
        if (status != unmarshal_success) {
            return status;
        }

        p_.x_.x_.x_.mont_encode(p_.x_.x_.x_);
        p_.x_.x_.y_.mont_encode(p_.x_.x_.y_);
        p_.x_.y_.x_.mont_encode(p_.x_.y_.x_);
        p_.x_.y_.y_.mont_encode(p_.x_.y_.y_);
        p_.x_.z_.x_.mont_encode(p_.x_.z_.x_);
        p_.x_.z_.y_.mont_encode(p_.x_.z_.y_);
        p_.y_.x_.x_.mont_encode(p_.y_.x_.x_);
        p_.y_.x_.y_.mont_encode(p_.y_.x_.y_);
        p_.y_.y_.x_.mont_encode(p_.y_.y_.x_);
        p_.y_.y_.y_.mont_encode(p_.y_.y_.y_);
        p_.y_.z_.x_.mont_encode(p_.y_.z_.x_);
        p_.y_.z_.y_.mont_encode(p_.y_.z_.y_);

        return status;
    }

    bool gt::operator==(const gt& rhs) const {
        return p_ == rhs.p_;
    }

    bool gt::operator!=(const gt& rhs) const {
        return !(rhs == *this);
    }

    std::string gt::string() {
        std::stringstream ss;
        ss << "bn256.gt" << p_.string();
        return ss.str();
    }

    // pair calculates an Optimal Ate pairing.
    gt pair(const g1& g1, const g2& g2) {
        gt ret{};
        ret.set(optimal_ate(g2.p_, g1.p_));
        return ret;
    }

    // pairing_check calculates the Optimal Ate pairing for a set of points.
    bool pairing_check(std::vector<g1>& a, std::vector<g2>& b) {
        gfp12 acc{};
        acc.set_one();
        for (auto i = 0; i < a.size(); ++i) {
            if (a[i].p_.is_infinity() || b[i].p_.is_infinity()) {
                continue;
            }
            acc.mul(acc, miller(b[i].p_, a[i].p_));
        }
        return final_exponentiation(acc).is_one();
    }

    // miller applies Miller's algorithm, which is a bilinear function from
    // the source groups to F_p^12. miller(g1, g2).finalize() is equivalent
    // to pair(g1,g2).
    gt miller(const g1& g1, const g2& g2) {
        gt ret{};
        ret.set(miller(g2.p_, g1.p_));
        return ret;
    }

    std::tuple<int512_t,g1> ramdom_g1() {
        random_256 rand;
        auto k = rand.sample();
        g1 ret{};
        return std::tuple(k, ret.scalar_base_mult(k));
    }

    std::tuple<int512_t,g2> ramdom_g2() {
        random_256 rand;
        g2 ret{};
        auto k = rand.sample();
        return std::make_tuple(k, ret.scalar_base_mult(k));
    }
}
