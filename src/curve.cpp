#include <curve.h>
#include <gfp_generic.h>
#include <constants.h>
#include <lattice.h>

using boost::multiprecision::int256_t;
namespace bn256 {

    static constexpr gfp curve_b {3};

    void curve_point::set(const curve_point& a) {
        x_.set(a.x_);
        y_.set(a.y_);
        z_.set(a.z_);
        t_.set(a.t_);
    }

    bool curve_point::is_on_curve() {
        make_affine();
        if (is_infinity()) {
            return true;
        }
        gfp y2_{}, x3_{};
        gfp_mul(y2_, y_, y_ );
        gfp_mul(x3_, x_, x_ );
        gfp_mul(x3_, x3_, x_ );
        gfp_mul(x3_, x3_, curve_b );

        return y2_ == x3_;
    }

    void curve_point::set_infinity() {
        x_.fill(0);
        y_ = new_gfp(1ll);
        z_.fill(0);
        t_.fill(0);
    }

    bool curve_point::is_infinity() const {
        constexpr gfp gfp_infinity {};
        return z_ == gfp_infinity;
    }

    void curve_point::add(const curve_point& a, const curve_point& b) {
        if (a.is_infinity()) {
            set(b);
            return;
        }
        if (b.is_infinity()) {
            set(a);
            return;
        }
        // See http://hyperelliptic.org/EFD/g1p/auto-code/shortw/jacobian-0/addition/add-2007-bl.op3

        // Normalize the points by replacing a = [x1:y1:z1] and b = [x2:y2:z2]
        // by [u1:s1:z1·z2] and [u2:s2:z1·z2]
        // where u1 = x1·z2², s1 = y1·z2³ and u1 = x2·z1², s2 = y2·z1³
        gfp z12{}, z22{};
        gfp_mul(z12, a.z_, a.z_);
        gfp_mul(z22, b.z_, b.z_);

        gfp u1{}, u2{};
        gfp_mul(u1, a.x_, z22);
        gfp_mul(u2, b.x_, z12);

        gfp t{}, s1{};
        gfp_mul(t, b.z_, z22);
        gfp_mul(s1, a.y_, t);

        gfp s2{};
        gfp_mul(t, a.z_, z12);
        gfp_mul(s2, b.y_, t);

        // Compute x_ = (2h)²(s²-u1-u2)
        // where s = (s2-s1)/(u2-u1) is the slope of the line through
        // (u1,s1) and (u2,s2). The extra factor 2h = 2(u2-u1) comes from the value of z_ below.
        // This is also:
        // 4(s2-s1)² - 4h²(u1+u2) = 4(s2-s1)² - 4h³ - 4h²(2u1)
        //                        = r² - j - 2v
        // with the notations below.
        gfp h{};
        gfp_sub(h, u2, u1);

        constexpr gfp gfp_zero{};
        bool x_equal = (h == gfp_zero);

        gfp_add(t, h, h);
        // i = 4h²
        gfp i{};
        gfp_mul(i, t, t);
        // j = 4h³
        gfp j{};
        gfp_mul(j, h, i);

        gfp_sub(t, s2, s1);
        bool y_equal = (t == gfp_zero);
        if (x_equal && y_equal) {
            double_(a);
            return;
        }
        gfp r{};
        gfp_add(r, t, t);

        gfp v{};
        gfp_mul(v, u1, i);

        // t4 = 4(s2-s1)²
        gfp t4{}, t6{};
        gfp_mul(t4, r, r);
        gfp_add(t, v, v);
        gfp_sub(t6, t4, j);

        gfp_sub(x_, t6, t);

        // Set y_ = -(2h)³(s1 + s*(x_/4h²-u1))
        // This is also
        // y_ = - 2·s1·j - (s2-s1)(2x - 2i·u1) = r(v-x_) - 2·s1·j
        gfp_sub(t, v, x_);   // t7
        gfp_mul(t4, s1, j);  // t8
        gfp_add(t6, t4, t4); // t9
        gfp_mul(t4, r, t);   // t10
        gfp_sub(y_, t4, t6);

        // Set z_ = 2(u2-u1)·z1·z2 = 2h·z1·z2
        gfp_add(t, a.z_, b.z_);       // t11
        gfp_mul(t4, t, t);      // t12
        gfp_sub(t, t4, z12);    // t13
        gfp_sub(t4, t, z22);    // t14
        gfp_mul(z_, t4, h);

    }

    void curve_point::double_(const curve_point& point) {
        // See http://hyperelliptic.org/EFD/g1p/auto-code/shortw/jacobian-0/doubling/dbl-2009-l.op3
        gfp a{}, b{}, c{}, d{}, e{}, f{}, t{}, t2{};

        gfp_mul(a, point.x_, point.x_);
        gfp_mul(b, point.y_, point.y_);
        gfp_mul(c, b, b);

        gfp_add(t, point.x_, b);
        gfp_mul(t2, t, t);
        gfp_sub(t, t2, a);
        gfp_sub(t2, t, c);

        gfp_add(d, t2, t2);
        gfp_add(t, a, a);
        gfp_add(e, t, a);
        gfp_mul(f, e, e);

        gfp_add(t, d, d);
        gfp_sub(x_, f, t);

        gfp_mul(z_, point.y_, point.z_);
        gfp_add(z_, z_, z_);

        gfp_add(t, c, c);
        gfp_add(t2, t, t);
        gfp_add(t, t2, t2);
        gfp_sub(y_, d, x_);
        gfp_mul(t2, e, y_);
        gfp_sub(y_, t2, t);
    }

    void curve_point::mul(const curve_point& a, const int256_t& scalar) {
        typedef std::array<curve_point, 4> curve_point_array_4_t;
        curve_point_array_4_t precomp{};

        precomp[1].set(a);
        precomp[2].set(a);
        gfp_mul(precomp[2].x_, precomp[2].x_, constants::xi_to_2p_squared_minus_2_over_3);
        precomp[3].add(precomp[1], precomp[2]);

        const auto multi_scalar = curve_lattice.multi(scalar);

        curve_point sum{};
        sum.set_infinity();
        curve_point t{};

        for (int i = multi_scalar.size() - 1; i >= 0; i--) {
            t.double_(sum);
            if (multi_scalar[i] == 0) {
                sum.set(t);
            } else {
                sum.add(t, precomp[multi_scalar[i]]);
            }
        }

        set(sum);
    }

    void curve_point::make_affine() {
        if (z_ == new_gfp(1ll)) {
            return;
        } else if (z_ == new_gfp(0ll)) {
            x_.fill(0);
            y_ = new_gfp(1ll);
            t_.fill(0);
            return;
        }
    }

    void curve_point::neg(const curve_point& a) {
        x_.set(a.x_);
        gfp_neg(y_, a.y_);
        z_.set(a.z_);
        t_.set(a.t_);
    }
}
