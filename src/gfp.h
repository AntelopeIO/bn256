#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <iosfwd>
#include <nonstd/span.hpp>
#include <system_error>
#include "gfp_generic.h"

namespace bn256 {

    typedef std::array<uint64_t, 4> uint64_array_4_t;
    typedef std::array<uint8_t, sizeof(uint64_array_4_t)> uint8_array_32_t;

    enum class unmarshal_error {
        COORDINATE_EXCEEDS_MODULUS = 1,
        COORDINATE_EQUALS_MODULUS,
        MALFORMED_POINT
    };

    namespace {
        struct unmarshal_error_category : std::error_category
        {
            const char* name() const noexcept override { return "unmarshall"; }
            std::string message(int ev) const override {
                switch (static_cast<unmarshal_error>(ev))
                {
                case unmarshal_error::COORDINATE_EXCEEDS_MODULUS:
                    return "coordinate exceeds modulus";
                case unmarshal_error::COORDINATE_EQUALS_MODULUS:
                    return "coordinate equal modulus";
                case unmarshal_error::MALFORMED_POINT:
                    return "malformed point";
                default:
                    return "(unrecognized error)";
                }
            }
        };
    }

    inline std::error_code make_error_code(unmarshal_error e) noexcept
    {
        static const unmarshal_error_category category;
        return {static_cast<int>(e), category};
    }


    struct gfp : std::array<uint64_t, 4> {

        void set_zero() noexcept;

        gfp invert() const noexcept;

        gfp neg() const noexcept {
            gfp r;
            gfp_neg(r, *this);
            return r;
        }

        gfp add(const gfp& other) const noexcept {
            gfp r;
            gfp_add(r, *this, other);
            return r;
        }

        gfp sub(const gfp& other) const noexcept {
            gfp r;
            gfp_sub(r, *this, other);
            return r;
        }

        gfp mul(const gfp& other) const noexcept {
            gfp r;
            gfp_mul(r, *this, other);
            return r;
        }

        void marshal(nonstd::span<uint8_t, 32> out) const noexcept;

        [[nodiscard]] std::error_code unmarshal(nonstd::span<const uint8_t, 32> input) noexcept;

        gfp mont_encode() const noexcept;

        gfp mont_decode() const noexcept;

        std::string string() const noexcept;
    };

    gfp new_gfp(int64_t x) noexcept;
    std::ostream& operator << (std::ostream& os, const gfp& v);
}

namespace std
{
  template <> struct is_error_code_enum<bn256::unmarshal_error> : true_type
  {
  };
}
