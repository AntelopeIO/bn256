#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <constants.h>

namespace bn256 {

    using namespace boost::multiprecision;

    struct lattice {
        std::vector<std::vector<int512_t>> vectors_;
        std::vector<int512_t> inverse_;
        const int512_t det_;

        [[nodiscard]] std::vector<int512_t> decompose(const int512_t& k) const;

        [[nodiscard]] std::vector<uint8_t> multi(const int512_t& scalar) const;

        static void round(int512_t& num, const int512_t& denom) ;
    };

    static const lattice curve_lattice = {
            .vectors_ = {
                    {
                            int512_t("147946756881789319000765030803803410728"),
                            int512_t("147946756881789319010696353538189108491")
                    },{
                            int512_t("147946756881789319020627676272574806254"),
                            int512_t("-147946756881789318990833708069417712965")
                    }
            },
            .inverse_ = {
                    int512_t("147946756881789318990833708069417712965"),
                    int512_t("147946756881789319010696353538189108491"),
            },
            .det_ = int512_t("43776485743678550444492811490514550177096728800832068687396408373151616991234")
    };

    static const lattice target_lattice = {
            .vectors_ = {
                    {
                            int512_t("9931322734385697761"),
                            int512_t("9931322734385697761"),
                            int512_t("9931322734385697763"),
                            int512_t("9931322734385697764")
                    },{
                            int512_t("4965661367192848881"),
                            int512_t("4965661367192848881"),
                            int512_t("4965661367192848882"),
                            int512_t("-9931322734385697762")
                    },{
                            int512_t("-9931322734385697762"),
                            int512_t("-4965661367192848881"),
                            int512_t("4965661367192848881"),
                            int512_t("-4965661367192848882")
                    },{
                            int512_t("9931322734385697763"),
                            int512_t("-4965661367192848881"),
                            int512_t("-4965661367192848881"),
                            int512_t("-4965661367192848881")
                    }
            },
            .inverse_ = {
                    int512_t("734653495049373973658254490726798021314063399421879442165"),
                    int512_t("147946756881789319000765030803803410728"),
                    int512_t("-147946756881789319005730692170996259609"),
                    int512_t("1469306990098747947464455738335385361643788813749140841702")
            },
            .det_ = constants::order
    };

}
