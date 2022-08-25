#include <gfp.h>
#include <constants.h>
#include <iostream>

namespace bn256 {

    void gfp_carry(gfP &a, const uint64_t head){
        gfP b{};
        uint64_t carry = 0;
        for (const auto& pi : p2) {
            const auto i = &pi - &p2[0];
            const auto ai = a[i];
            const auto bi = ai - pi - carry;
            b[i] = bi;
            carry = (pi & ~ai | (pi | ~ai) & bi) >> 63ull;
            std::cout << i << "," << carry << "," << ai << "," << pi << "," << bi << std::endl;
        }
        carry = carry & ~head;

        // If b is negative, then return a.
        // Else return b.
        carry = -carry;
        const auto ncarry = ~carry;
        for (auto i = 0; i < 4; i++) {
            a[i] = (a[i] & carry) | (b[i] & ncarry);
        }
    }

    void gfp_neg(gfP& c, const gfP& a){
        uint64_t carry = 0;
        for (const auto& pi : p2) {
            const auto i = &pi - &p2[0];
            const auto ai = a[i];
            const auto ci = pi - ai - carry;
            c[i] = ci;
            carry = (ai & ~pi | (ai | ~pi) & ci) >> 63ull;
        }
        gfp_carry(c, 0);
    }

    void gfp_add(gfP& c, const gfP& a, const gfP& b){
        uint64_t carry = 0;
        for (const auto& ai : a) {
            const auto i = &ai - &a[0];
            const auto bi = b[i];
            const auto ci = ai + bi + carry;
            c[i] = ci;
            carry = (ai & bi | (ai | bi) & ~ci) >> 63ull;
        }
        gfp_carry(c, carry);
    }

    void gfp_sub(gfP& c, const gfP& a, const gfP& b){
        uint64_t carry = 0;
        gfP t{};
        for (const auto& pi : p2) {
            const auto i = &pi - &p2[0];
            const auto bi = b[i];
            const auto ti = pi - bi - carry;
            t[i] = ti;
            carry = (bi & ~pi | (bi | ~pi) & ti) >> 63ull;
        }
        carry = 0;
        for (const auto& ai : a) {
            const auto i = &ai - &a[0];
            auto ti = t[i];
            auto ci = ai + ti + carry;
            c[i] = ci;
            carry = (ai & ti | (ai | ti) & ~ci) >> 63ull;
        }
        gfp_carry(c, carry);
    }

    uint64_array_8_t& mul(const gfP& a, const gfP& b, uint64_array_8_t& rc){
        constexpr uint64_t mask16 = 0x0000ffff;
        constexpr uint64_t mask32 = 0xffffffff;
        uint64_array_32_t array32{};

        for (const auto& ai : a) {
            const uint64_t i = &ai - &a[0];
            const uint64_t a0 = ai & mask16;
            const uint64_t a1 = (ai >> 16ull) & mask16;
            const uint64_t a2 = (ai >> 32ull) & mask16;
            const uint64_t a3 = (ai >> 48ull);
            for (const auto& bj : b) {
                const uint64_t j = &bj - &b[0];
                const uint64_t b0 = bj & mask32;
                const uint64_t b2 = bj >> 32ull;
                const uint64_t off = 4ull * (i + j);
                array32[off + 0] += a0 * b0;
                array32[off + 1] += a1 * b0;
                array32[off + 2] += a2 * b0 + a0 * b2;
                array32[off + 3] += a3 * b0 + a1 * b2;
                array32[off + 4] += a2 * b2;
                array32[off + 5] += a3 * b2;
            }
        }
        for (uint64_t i = 1ull; i < 4ull; i++) {
            const auto shift = 16ull * i;
            uint64_t head = 0ull, carry = 0ull;
            for (uint64_t j = 0ull; j < 8ull; j++) {
                const auto block = 4ull * j;
                const auto xi = array32[block];
                const auto yi = (array32[block + i] << shift) + head;
                const auto zi = xi + yi + carry;
                array32[block] = zi;
                carry = (xi & yi | (xi | yi) & ~zi) >> 63ull;
                head = array32[block + i] >> (64ull - shift);
            }
        }
        rc = {
                array32[0],
                array32[4],
                array32[8],
                array32[12],
                array32[16],
                array32[20],
                array32[24],
                array32[28]
        };
        return rc;
    }

    uint64_array_4_t& halfMul(const uint64_array_4_t& a, const uint64_array_4_t& b, uint64_array_4_t& rc) {
        constexpr uint64_t mask16 = 0x0000ffff;
        constexpr uint64_t mask32 = 0xffffffff;
        uint64_array_18_t array18{};
        for (const auto& ai : a) {
            const uint64_t i = &ai - &a[0];
            const uint64_t a0 = ai & mask16;
            const uint64_t a1 = (ai >> 16ull) & mask16;
            const uint64_t a2 = (ai >> 32ull) & mask16;
            const uint64_t a3 = (ai >> 48ull);
            for (const auto& bj : b) {
                const auto j = &bj - &b[0];
                if (i+j > 3) {
                    break;
                }
                const uint64_t b0 = bj & mask32;
                const uint64_t b2 = bj >> 32ull;
                const uint64_t off = 4 * (i + j);
                array18[off + 0] += a0 * b0;
                array18[off + 1] += a1 * b0;
                array18[off + 2] += a2 * b0 + a0 * b2;
                array18[off + 3] += a3 * b0 + a1 * b2;
                array18[off + 4] += a2 * b2;
                array18[off + 5] += a3 * b2;
            }
        }
        for (uint64_t i = 1ull; i < 4ull; i++) {
            const uint64_t shift = 16ull * i;
            uint64_t head = 0ull, carry = 0ull;
            for (uint64_t j = 0ull; j < 4ull; j++) {
                const uint64_t block = 4ull * j;
                const uint64_t xi = array18[block];
                const uint64_t yi = (array18[block + i] << shift) + head;
                const uint64_t zi = xi + yi + carry;
                array18[block] = zi;
                carry = (xi & yi | (xi | yi) & ~zi) >> 63ull;
                head = array18[block + i] >> (64ull - shift);
            }
        }
        rc =  {
            array18[0],
            array18[4],
            array18[8],
            array18[12]
        };
        return rc;
    }

    void gfp_mul(gfP& c, const gfP& a, const gfP& b) {
        uint64_array_8_t array8_T{};
        auto T = mul(a, b, array8_T);
        uint64_array_4_t array4_m{};
        const uint64_array_4_t array4_1 = {T[0], T[1], T[2], T[3]};
        const auto m = halfMul(array4_1, np, array4_m);
        uint64_array_8_t array8_t{};
        const uint64_array_4_t array4_2 = {m[0], m[1], m[2], m[3]};
        const auto t = mul(array4_2, p2, array8_t);
        uint64_t carry = 0;
        for (const auto& Ti : T) {
            const auto i = &Ti - &T[0];
            const auto ti = t[i];
            const auto zi = Ti + ti + carry;
            T[i] = zi;
            carry = (Ti & ti | (Ti | ti) & ~zi) >> 63ull;
        }
        c = {T[4], T[5], T[6], T[7]};
        std::cout << "c:" << T[4] << "," << T[5] << "," << T[6] << "," << T[7] << std::endl;
        gfp_carry(c, carry);
        std::cout << "c:" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << std::endl;
    }
}
