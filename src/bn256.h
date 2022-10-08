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

#include <boost/multiprecision/cpp_int.hpp>
#include <curve.h>
#include <gfp12.h>
#include <twist.h>
#include <utility>

using boost::multiprecision::int512_t;

namespace bn256 {

// curveGen is the generator of G₁.
const curve_point curve_gen = { new_gfp(1), new_gfp(2), new_gfp(1), new_gfp(1) };

static const twist_point twist_gen = {
   {
         { 0xafb4737da84c6140, 0x6043dd5a5802d8c4, 0x09e950fc52a02f86, 0x14fef0833aea7b6b },
         { 0x8e83b5d102bc2026, 0xdceb1935497b0172, 0xfbb8264797811adf, 0x19573841af96503b },
   },
   {
         { 0x64095b56c71856ee, 0xdc57f922327d3cbb, 0x55f935be33351076, 0x0da4a0e693fd6482 },
         { 0x619dfa9d886be9f6, 0xfe7fd297f59e9b78, 0xff9e1a62231b7dfe, 0x28fd7eebae9e4206 },
   },
   { new_gfp(0), new_gfp(1) },
   { new_gfp(0), new_gfp(1) }
};

// G1 is an abstract cyclic group. The zero value is suitable for use as the
// output of an operation, but cannot be used as an input.
struct g1 {
   curve_point p_;

   static g1 scalar_base_mult(const int512_t& k) noexcept;

   g1 scalar_mult(const int512_t& k) const noexcept;

   g1 add(const g1& b) const noexcept;

   g1 neg();

   void                    marshal(nonstd::span<uint8_t, 64> out) const noexcept;
   std::array<uint8_t, 64> marshal() const noexcept {
      std::array<uint8_t, 64> result;
      marshal(result);
      return result;
   }

   [[nodiscard]] std::error_code unmarshal(nonstd::span<const uint8_t, 64> in) noexcept;

   bool operator==(const g1& rhs) const noexcept { return p_ == rhs.p_; }
   bool operator!=(const g1& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};

inline std::ostream& operator<<(std::ostream& os, const g1& v) { return os << v.p_.string(); }

// g2 is an abstract cyclic group. The zero value is suitable for use as the
// output of an operation, but cannot be used as an input.
struct g2 {
   twist_point p_;

   static g2 scalar_base_mult(const int512_t& k) noexcept;

   g2 scalar_mult(const int512_t& k) const noexcept;

   g2 add(const g2& b) const noexcept;

   g2 neg() const noexcept;

   void                     marshal(nonstd::span<uint8_t, 128> out) const noexcept;
   std::array<uint8_t, 128> marshal() const noexcept {
      std::array<uint8_t, 128> result;
      marshal(result);
      return result;
   }

   [[nodiscard]] std::error_code unmarshal(nonstd::span<const uint8_t, 128> m) noexcept;

   bool operator==(const g2& rhs) const noexcept { return p_ == rhs.p_; }
   bool operator!=(const g2& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};

inline std::ostream& operator<<(std::ostream& os, const g2& v) { return os << v.p_.string(); }

// GT is an abstract cyclic group. The zero value is suitable for use as the
// output of an operation, but cannot be used as an input.
struct gt {
   gfp12 p_;

   gt scalar_mult(const int512_t& k) const noexcept;

   gt add(const gt& b) const noexcept ;

   gt neg() const noexcept;

   gt finalize() const noexcept;

   // marshal converts g2 to a byte slice.
   void                     marshal(nonstd::span<uint8_t, 384> out) const noexcept;
   std::array<uint8_t, 384> marshal() const noexcept {
      std::array<uint8_t, 384> result;
      marshal(result);
      return result;
   }

   // unmarshal sets gt to the result of converting the output of marshal back into
   // a group element and then returns unmarshal_status.
   [[nodiscard]] std::error_code unmarshal(nonstd::span<const uint8_t, 384> m) noexcept;

   bool operator==(const gt& rhs) const noexcept { return p_ == rhs.p_; }
   bool operator!=(const gt& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};

std::ostream& operator<<(std::ostream& os, const gt& v);

// pair calculates an Optimal Ate pairing.
gt pair(const g1& g1, const g2& g2) noexcept;

// pairing_check calculates the Optimal Ate pairing for a set of points.
bool pairing_check(std::vector<g1>& a, std::vector<g2>& b) noexcept;

// miller applies Miller's algorithm, which is a bilinear function from
// the source groups to F_p^12. miller(g1, g2).finalize() is equivalent
// to pair(g1,g2).
gt miller(const g1& g1, const g2& g2) noexcept;

std::tuple<int512_t, g1> ramdom_g1();
std::tuple<int512_t, g2> ramdom_g2();

} // namespace bn256
