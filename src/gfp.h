#pragma once
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

    void gfp_invert(gfp& e, const gfp& f);

    void marshal(gfp& e, std::vector<uint8_t>& out);
    std::optional<std::string> unmarshal(gfp& e, std::vector<uint8_t>& in);

    void mont_encode(gfp& c, const gfp& a);
    void mont_decode(gfp& c, const gfp& a);

}
