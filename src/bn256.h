#pragma once
// Package bn256 implements a particular bilinear group at the 128-bit security
// level.
//
// Bilinear groups are the basis of many of the new cryptographic protocols that
// have been proposed over the past decade. They consist of a triplet of groups
// (G₁, G₂ and GT) such that there exists a function e(g₁ˣ,g₂ʸ)=gTˣʸ (where gₓ
// is a generator of the respective group). That function is called a pairing
// function.
//
// This package specifically implements the Optimal Ate pairing over a 256-bit
// Barreto-Naehrig curve as described in
// http://cryptojedi.org/papers/dclxvi-20100714.pdf. Its output is not
// compatible with the implementation described in that paper, as different
// parameters are chosen.
//
// (This package previously claimed to operate at a 128-bit security level.
// However, recent improvements in attacks mean that is no longer true. See
// https://moderncrypto.org/mail-archive/curves/2016/000740.html.)

#include <curve.h>
#include <twist.h>
#include <gfp12.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <utility>

using boost::multiprecision::int256_t;

namespace bn256 {

    // curveGen is the generator of G₁.
    inline constexpr curve_point curve_gen = {1, 2, 1, 1};

    static const twist_point twist_gen = {{{0xafb4737da84c6140, 0x6043dd5a5802d8c4, 0x09e950fc52a02f86, 0x14fef0833aea7b6b},{0x8e83b5d102bc2026, 0xdceb1935497b0172, 0xfbb8264797811adf, 0x19573841af96503b},},{{0x64095b56c71856ee, 0xdc57f922327d3cbb, 0x55f935be33351076, 0x0da4a0e693fd6482},{0x619dfa9d886be9f6, 0xfe7fd297f59e9b78, 0xff9e1a62231b7dfe, 0x28fd7eebae9e4206},},{new_gfp(0), new_gfp(1)},{new_gfp(0), new_gfp(1)}};

    typedef std::array<uint8_array_32_t, 2> uint8_array_32_array_2_t;
    typedef std::array<uint8_array_32_t, 4> uint8_array_32_array_4_t;
    typedef std::array<uint8_array_32_t, 12> uint8_array_32_array_12_t;

    // G1 is an abstract cyclic group. The zero value is suitable for use as the
    // output of an operation, but cannot be used as an input.
    struct g1 {
        curve_point p_ {};

        const g1& scalar_base_mult(const int256_t& k);

        const g1& scalar_mult(const g1& a, const int256_t& k);

        const g1& add(const g1& a, const g1& b);

        const g1& neg(const g1& a);

        const g1& set(const g1& a);

        [[nodiscard]] uint8_array_32_array_2_t marshal();

        unmarshal_status unmarshal(uint8_array_32_array_2_t& m);
    };

    // g2 is an abstract cyclic group. The zero value is suitable for use as the
    // output of an operation, but cannot be used as an input.
    struct g2 {
        twist_point p_ {};

        const g2& scalar_base_mult(const int256_t& k);

        const g2& scalar_mult(const g2& a, const int256_t& k);

        const g2& add(const g2& a, const g2& b);

        const g2& neg(const g2& a);

        const g2& set(const g2& a);

        [[nodiscard]] uint8_array_32_array_4_t marshal();

        unmarshal_status unmarshal(uint8_array_32_array_4_t& m);

    };

    // GT is an abstract cyclic group. The zero value is suitable for use as the
    // output of an operation, but cannot be used as an input.
    struct gt {
        gfp12 p_ {};

        // scalar_mult sets gt to a*k and then returns gt.
        const gt& scalar_mult(const gt& a, const int256_t& k);

        // add sets gt to a+b and then returns gt.
        const gt& add(const gt& a, const gt& b);

        // neg sets g2 to -a and then returns g2.
        const gt& neg(const gt& a);

        // set sets gt to a and then returns gt.
        const gt& set(const gt& a);

        const gt& set(const gfp12& a);

        // finalize is a linear function from F_p^12 to gt.
        const gt& finalize();

        // marshal converts g2 to a byte slice.
        [[nodiscard]] uint8_array_32_array_12_t marshal() const;

        // unmarshal sets gt to the result of converting the output of marshal back into
        // a group element and then returns unmarshal_status.
        unmarshal_status unmarshal(uint8_array_32_array_12_t& m);

        bool operator==(const gt& rhs) const;

        bool operator!=(const gt& rhs) const;

    };

    // pair calculates an Optimal Ate pairing.
    gt pair(const g1& g1, const g2& g2);

    // pairing_check calculates the Optimal Ate pairing for a set of points.
    bool pairing_check(std::vector<g1>& a, std::vector<g2>& b);

    // miller applies Miller's algorithm, which is a bilinear function from
    // the source groups to F_p^12. miller(g1, g2).finalize() is equivalent
    // to pair(g1,g2).
    gt miller(const g1& g1, const g2& g2);

    std::tuple<int256_t,g1> ramdom_g1();
    std::tuple<int256_t,g2> ramdom_g2();

}
