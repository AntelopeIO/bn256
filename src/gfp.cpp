#include <gfp.h>
#include <gfp_generic.h>
#include <constants.h>

namespace bn256 {

    gfp new_gfp(int64_t x) {
        gfp out{};
        if (x >= 0) {
            out = {uint64_t(x)};
        } else {
            out = {uint64_t(-x)};
            gfp_neg(out, out);
        }
        mont_encode(out, out);
        return out;
    }

    void gfp_invert(gfp& e, const gfp& f) {

        constexpr uint64_array_4_t bits = {
            0x3c208c16d87cfd45,
            0x97816a916871ca8d,
            0xb85045b68181585d,
            0x30644e72e131a029
        };

        gfp sum{}, power{};
        gfp_set(sum, constants::rn1);
        gfp_set(power, f);

        for (auto word = 0; word < bits.size(); word++) {
            for (auto bit = 0; bit < 64; bit++) {
                if (((bits[word] >> bit) & 1) == 1) {
                    gfp_mul(sum, sum, power);
                }
                gfp_mul(power, power, power);
            }
        }

        gfp_mul(sum, sum, constants::r3);
        gfp_set(e,sum);
    }

    void marshal(gfp& e, std::vector<uint8_t>& out) {
        for (auto w = 0; w < 4; w++) {
            for (auto b = 0; b < 8; b++) {
                uint8_t t = (e[3-w] >> (56 - 8*b));
                out[8*w*b] = t;
            }
        }
    }

    std::optional<std::string> unmarshal(gfp& e, std::vector<uint8_t>& in) {
        // Unmarshal the bytes into little endian form
        for (auto w = 0; w < 4; w++) {
            e[3-w] = 0;
            for (auto b = 0; b < 8; b++) {
                e[3-w] += uint64_t(in[8*w+b]) << (56 - 8*b);
            }
        }

        // Ensure the point respects the curve modulus
        const std::string error_string("bn256: coordinate exceeds modulus");

        for (auto i = 3; i >= 0; i--) {
            if (e[i] < constants::p2[i]) {
                return std::nullopt;
            }
            if (e[i] > constants::p2[i]) {
                return error_string;
            }
        }
        return error_string;
    }

    void mont_encode(gfp& c, const gfp& a){
        gfp_mul(c, a, constants::r2);
    }

    void mont_decode(gfp& c, const gfp& a){
        constexpr gfp decode_b {1};
        gfp_mul(c, a, decode_b);
    }
}