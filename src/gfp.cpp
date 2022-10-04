#include <gfp.h>
#include <gfp_generic.h>
#include <constants.h>
#include <iostream>

namespace bn256 {

   void gfp::set(const gfp& f) {
      (*this) = f;
   }

   void gfp::set_zero() {
      this->fill(0);
   }

   void gfp::invert(const gfp& f) {
      constexpr uint64_array_4_t bits = {
         0x3c208c16d87cfd45,
         0x97816a916871ca8d,
         0xb85045b68181585d,
         0x30644e72e131a029
      };

      gfp sum{}, power{};
      sum.set(constants::rn1);
      power.set(f);

      for (auto word = 0; word < bits.size(); word++) {
         for (auto bit = 0; bit < 64; bit++) {
            if (((bits[word] >> bit) & 1) == 1) {
               gfp_mul(sum, sum, power);
            }
            gfp_mul(power, power, power);
         }
      }

      gfp_mul(sum, sum, constants::r3);
      this->set(sum);
   }

   void gfp::marshal(uint8_array_32_t& out) const {
      for (auto w = 0; w < 4; w++) {
         for (auto b = 0; b < 8; b++) {
            uint8_t t = ((*this)[3-w] >> (56 - 8*b));
            out[8*w+b] = t;
         }
      }
   }

   unmarshal_status gfp::unmarshal(uint8_array_32_t& in) {
      // Unmarshal the bytes into little endian form
      for (auto w = 0; w < 4; w++) {
         (*this)[3-w] = 0;
         for (auto b = 0; b < 8; b++) {
            (*this)[3-w] += uint64_t(in[8*w+b]) << (56 - 8*b);
         }
      }

      // Ensure the point respects the curve modulus
      for (auto i = 3; i >= 0; i--) {
         if ((*this)[i] < constants::p2[i]) {
            return unmarshal_success;
         }
         if ((*this)[i] > constants::p2[i]) {
            return unmarshal_coordinate_exceeds_modulus;
         }
      }
      return unmarshal_coordinate_exceeds_modulus;
   }

   void gfp::mont_encode(const gfp& a) {
      gfp_mul(*this, a, constants::r2);
   }

   void gfp::mont_decode(const gfp& a) {
      constexpr gfp decode_b{1};
      gfp_mul(*this, a, decode_b);
   }

   std::string gfp::string() const {
      std::string result;
      result.resize(64);

      std::for_each(rbegin(), rend(), [buf = result.data()](uint64_t v) mutable{
         const char hex_table[]= "0123456789abcdef";
         unsigned char* p = reinterpret_cast<unsigned char*>(&v) + 8;
         for (int i = 0; i < sizeof(uint64_t); ++i) {
            unsigned x = *(--p);
            *buf++ = hex_table[(x >> 4)];
            *buf++ = hex_table[x & 0x0F];
         }
      });
      return result;
   }

   gfp new_gfp(int64_t x) {
      gfp out{};
      if (x >= 0) {
         out = {uint64_t(x)};
      } else {
         out = {uint64_t(-x)};
         gfp_neg(out, out);
      }
      out.mont_encode(out);
      return out;
   }

   std::ostream& operator << (std::ostream& os, const gfp& v) {
      os << v.string();
      return os;
   }
}
