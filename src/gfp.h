#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <optional>
#include <string>

namespace bn256 {

    typedef std::array<uint64_t, 4> uint64_array_4_t;

    struct gfp : public std::array<uint64_t, 4> {

        void set(const gfp& f);

        void set_zero();

        void invert(const gfp& f);

        void marshal(std::vector<uint8_t>& out);

        std::optional<std::string> unmarshal(std::vector<uint8_t>& in);

        void mont_encode(const gfp& a);

        void mont_decode(const gfp& a);

    };

    gfp new_gfp(int64_t x);
}
