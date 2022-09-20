#pragma once
#include <gfp2.h>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::int512_t;

namespace bn256 {
    struct twist_point {
        gfp2 x_;
        gfp2 y_;
        gfp2 z_;
        gfp2 t_;

        void set(const twist_point& a);

        bool is_on_curve();

        void set_infinity();

        [[nodiscard]] bool is_infinity() const;

        void add(const twist_point& a, const twist_point& b);

        void double_(const twist_point& a);

        void mul(const twist_point& a, const int512_t& scalar);

        void make_affine();

        void neg(const twist_point& a);

        std::string string();

    };

}
