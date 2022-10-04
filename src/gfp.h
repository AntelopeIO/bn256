#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <iosfwd>

namespace bn256 {

    typedef std::array<uint64_t, 4> uint64_array_4_t;
    typedef std::array<uint8_t, sizeof(uint64_array_4_t)> uint8_array_32_t;

    enum unmarshal_status {
        unmarshal_success,
        unmarshal_coordinate_exceeds_modulus,
        unmarshal_malformed_point
    };

    struct gfp : public std::array<uint64_t, 4> {

        void set(const gfp& f);

        void set_zero();

        void invert(const gfp& f);

        void marshal(uint8_array_32_t& out) const;

        unmarshal_status unmarshal(uint8_array_32_t& in);

        void mont_encode(const gfp& a);

        void mont_decode(const gfp& a);

        std::string string() const ;
    };

    gfp new_gfp(int64_t x);
    std::ostream& operator << (std::ostream& os, const gfp& v);
}
