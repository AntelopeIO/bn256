#ifndef BN256_LATTICE_H
#define BN256_LATTICE_H

#include <boost/multiprecision/cpp_int.hpp>
#include <constants.h>

using boost::multiprecision::int256_t;

namespace bn256 {

    std::size_t bitlen(const int256_t& value);

    struct lattice {
        std::vector<std::vector<int256_t>> vectors_;
        std::vector<int256_t> inverse_;
        const int256_t det_;

        [[nodiscard]] std::vector<int256_t> decompose(const int256_t& k) const;

        [[nodiscard]] std::vector<uint8_t> multi(const int256_t& scalar) const;

        void round(int256_t& num, const int256_t& denom) const;
    };

    static const lattice curve_lattice = {
            .vectors_ = {
                    {
                            int256_t("147946756881789319000765030803803410728"),
                            int256_t("147946756881789319010696353538189108491")
                    },{
                            int256_t("147946756881789319020627676272574806254"),
                            int256_t("-147946756881789318990833708069417712965")
                    }
            },
            .inverse_ = {
                    int256_t("147946756881789318990833708069417712965"),
                    int256_t("147946756881789319010696353538189108491"),
            },
            .det_ = int256_t("43776485743678550444492811490514550177096728800832068687396408373151616991234")
    };

    static const lattice target_lattice = {
            .vectors_ = {
                    {
                            int256_t("9931322734385697761"),
                            int256_t("9931322734385697761"),
                            int256_t("9931322734385697763"),
                            int256_t("9931322734385697764")
                    },{
                            int256_t("4965661367192848881"),
                            int256_t("4965661367192848881"),
                            int256_t("4965661367192848882"),
                            int256_t("-9931322734385697762")
                    },{
                            int256_t("-9931322734385697762"),
                            int256_t("-4965661367192848881"),
                            int256_t("4965661367192848881"),
                            int256_t("-4965661367192848882")
                    },{
                            int256_t("9931322734385697763"),
                            int256_t("-4965661367192848881"),
                            int256_t("-4965661367192848881"),
                            int256_t("-4965661367192848881")
                    }
            },
            .inverse_ = {
                    int256_t("734653495049373973658254490726798021314063399421879442165"),
                    int256_t("147946756881789319000765030803803410728"),
                    int256_t("-147946756881789319005730692170996259609"),
                    int256_t("1469306990098747947464455738335385361643788813749140841702")
            },
            .det_ = abs(order)
    };

}


#endif //BN256_LATTICE_H
