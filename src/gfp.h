#ifndef BN256_GFP_H
#define BN256_GFP_H

#include <cstdint>
#include <array>
#include <vector>
#include <optional>
#include <string>

namespace bn256 {

    typedef std::array<uint64_t, 4> uint64_array_4_t;
    typedef uint64_array_4_t gfp;

    static void gfp_set(gfp& e, const gfp& f) {
        e = f;
    }

    static void gfp_setzero(gfp& e) {
        e.fill(0);
    }

    gfp new_gfp(int64_t x);

    static void invert(gfp& e, gfp& f);

    void marshal(gfp& e, std::vector<uint8_t>& out);
    std::optional<std::string> unmarshal(gfp& e, std::vector<uint8_t>& in);

    static void mont_encode(gfp& c, gfp& a);
    static void mont_decode(gfp& c, gfp& a);

}

#endif //BN256_GFP_H
