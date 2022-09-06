#include <twist.h>
#include <constants.h>
#include <lattice.h>

namespace bn256 {

    constexpr gfp2 twist_b = {
            {0x38e7ecccd1dcff67, 0x65f0b37d93ce0d3e, 0xd749d0dd22ac00aa, 0x0141b9ce4a688d4d},
            {0x3bf938e377b802a8, 0x020b1b273633535d, 0x26b7edf049755260, 0x2514c6324384a86d},
    };

    void twist_point::set(twist_point& a) {
        x_.set(a.x_);
        y_.set(a.y_);
        z_.set(a.z_);
        t_.set(a.t_);
    }

    bool twist_point::is_on_curve() {
        make_affine();
        if (is_infinity()) {
            return true;
        }

        gfp2 y2{}, x3{};
        y2.square(y_);
        x3.square(x_);
        x3.mul(x3, x_);
        x3.add(x3, twist_b);

        if (y2 != x3) {
            return false;
        }

        twist_point cneg{};
        cneg.mul(*this, constants::order);
        return cneg.z_.is_zero();
    }

    void twist_point::set_infinity() {
        x_.set_zero();
        y_.set_one();
        z_.set_zero();
        t_.set_zero();
    }

    bool twist_point::is_infinity() const {
        return z_.is_zero();
    }

    void twist_point::add(twist_point& a, twist_point& b) {
        if (a.is_infinity()) {
            set(b);
            return;
        }
        if (b.is_infinity()) {
            set(a);
            return;
        }

        // See http://hyperelliptic.org/EFD/g1p/auto-code/shortw/jacobian-0/addition/add-2007-bl.op3
        gfp2 z12{}, z22{}, u1{}, u2{}, t{}, s1{}, s2{}, h{}, i{}, j{}, r{}, v{}, t4{}, t6{};
        z12.square(a.z_);
        z22.square(a.z_);
        u1.mul(a.x_, z22);
        u2.mul(b.x_, z12);

        t.mul(b.z_, z22);
        s1.mul(a.y_, t);

        t.mul(a.z_, z12);
        s2.mul(b.y_, t);

        h.sub(u2, u1);
        bool x_equal = h.is_zero();

        t.add(h, h);
        i.square(t);
        j.mul(h, i);

        t.sub(s2, s1);
        bool y_equal = t.is_zero();
        if (x_equal && y_equal) {
            double_(a);
            return;
        }
        r.add(t, t);

        v.mul(u1, i);

        t4.square(r);
        t.add(v, v);
        t6.sub(t4, j);
        x_.sub(t6, t);

        t.sub(v, x_);   // t7
        t4.mul(s1, j);  // t8
        t6.add(t4, t4); // t9
        t4.mul(r, t);   // t10
        y_.sub(t4, t6);

        t.add(z_, b.z_); // t11
        t4.square(t);      // t12
        t.sub(t4, z12);  // t13
        t4.sub(t, z22);  // t14
        z_.mul(t4, h);

    }

    void twist_point::double_(twist_point& a) {
        // See http://hyperelliptic.org/EFD/g1p/auto-code/shortw/jacobian-0/doubling/dbl-2009-l.op3
        gfp2 a2{}, b{}, c{}, t{}, t2{}, d{}, e{}, f{};
        a2.square(a.x_);
        b.square(a.y_);
        c.square(b);

        t.add(a.x_, b);
        t2.square(t);
        t.sub(t2, a2);
        t2.sub(t, c);
        d.add(t2, t2);
        t.add(a2, a2);
        e.add(t, a2);
        f.square(e);

        t.add(d, d);
        x_.sub(f, t);

        z_.mul(a.y_, a.z_);
        z_.add(z_, z_);

        t.add(c, c);
        t2.add(t, t);
        t.add(t2, t2);
        y_.sub(d, x_);
        t2.mul(e, y_);
        y_.sub(t2, t);

    }

    void twist_point::mul(twist_point& a, const int128_t& scalar) {
        twist_point sum{}, t{};

        for (int i = bitlen(scalar); i >= 0; i--) {
            t.double_(sum);
            if (bit_test(scalar,i) ) {
                sum.add(t, a);
            } else {
                sum.set(t);
            }
        }

        set(sum);

    }

    void twist_point::make_affine() {
        if (z_.is_one()) {
            return;
        } else if (z_.is_zero()) {
            x_.set_zero();
            y_.set_zero();
            t_.set_zero();
            return;
        }
        gfp2 z_inv{}, t{}, z_inv2{};
        z_inv.invert(z_);
        t.mul(y_, z_inv);
        z_inv2.square(z_inv);
        y_.mul(t, z_inv2);
        t.mul(x_, z_inv2);
        x_.set(t);
        z_.set_one();
        t_.set_one();

    }

    void twist_point::neg(twist_point& a) {
        x_.set(a.x_);
        y_.neg(a.y_);
        z_.set(a.z_);
        t_.set_zero();
    }
}