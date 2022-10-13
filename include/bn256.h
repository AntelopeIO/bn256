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
#include <utility>
#include <array>
#include <system_error>
#include "span.h"

using boost::multiprecision::int512_t;

namespace bn256 {

struct curve_point;
struct twist_point;
struct gfp12;

// G1 is an abstract cyclic group. The zero value is suitable for use as the
// output of an operation, but cannot be used as an input.
class g1 {
   uint64_t p_[4 * 4];

 public:
   g1() = default;
   explicit g1(const curve_point&);

   static g1 curve_gen;

   curve_point& p();
   const curve_point& p() const;

   static g1 scalar_base_mult(const int512_t& k) noexcept;

   g1 scalar_mult(const int512_t& k) const noexcept;

   g1 add(const g1& b) const noexcept;

   g1 neg();

   void                    marshal(std::span<uint8_t, 64> out) const noexcept;
   std::array<uint8_t, 64> marshal() const noexcept {
      std::array<uint8_t, 64> result;
      marshal(result);
      return result;
   }

   [[nodiscard]] std::error_code unmarshal(std::span<const uint8_t, 64> in) noexcept;

   bool operator==(const g1& rhs) const noexcept { return memcmp(p_, rhs.p_, sizeof(*this)) == 0; }
   bool operator!=(const g1& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};

inline std::ostream& operator<<(std::ostream& os, const g1& v) { return os << v.string(); }

// g2 is an abstract cyclic group. The zero value is suitable for use as the
// output of an operation, but cannot be used as an input.
class g2 {
   uint64_t p_[4 * 2 * 4];

 public:
   g2() = default;
   explicit g2(const twist_point&);

   static g2 twist_gen;

   twist_point& p();
   const twist_point& p() const;

   static g2 scalar_base_mult(const int512_t& k) noexcept;

   g2 scalar_mult(const int512_t& k) const noexcept;

   g2 add(const g2& b) const noexcept;

   g2 neg() const noexcept;

   void                     marshal(std::span<uint8_t, 128> out) const noexcept;
   std::array<uint8_t, 128> marshal() const noexcept {
      std::array<uint8_t, 128> result;
      marshal(result);
      return result;
   }

   [[nodiscard]] std::error_code unmarshal(std::span<const uint8_t, 128> m) noexcept;

   bool operator==(const g2& rhs) const noexcept { return memcmp(p_, rhs.p_, sizeof(*this)) == 0; }
   bool operator!=(const g2& rhs) const noexcept { return !(*this == rhs); }

   std::string string() const;
};

inline std::ostream& operator<<(std::ostream& os, const g2& v) { return os << v.string(); }

// GT is an abstract cyclic group. The zero value is suitable for use as the
// output of an operation, but cannot be used as an input.
class gt {

   uint64_t p_[4 * 2 * 3 * 2];

 public:
   gt() = default;
   explicit gt(const gfp12&);

   gfp12& p();
   const gfp12& p() const;

   gt scalar_mult(const int512_t& k) const noexcept;

   gt add(const gt& b) const noexcept;

   gt neg() const noexcept;

   gt finalize() const noexcept;

   // marshal converts g2 to a byte slice.
   void                     marshal(std::span<uint8_t, 384> out) const noexcept;
   std::array<uint8_t, 384> marshal() const noexcept {
      std::array<uint8_t, 384> result;
      marshal(result);
      return result;
   }

   // unmarshal sets gt to the result of converting the output of marshal back into
   // a group element and then returns unmarshal_status.
   [[nodiscard]] std::error_code unmarshal(std::span<const uint8_t, 384> m) noexcept;

   bool operator==(const gt& rhs) const noexcept { return memcmp(p_, rhs.p_, sizeof(*this)) == 0; }
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
