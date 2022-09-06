
#include <gfp_generic.h>
#include <constants.h>
#include <gfp.h>

namespace bn256 {

    typedef std::array<uint64_t, 8> uint64_array_8_t;
    typedef std::array<uint64_t, 18> uint64_array_18_t;
    typedef std::array<uint64_t, 32> uint64_array_32_t;

    static void gfp_carry(gfp& a, uint64_t head){
        gfp b{};
        uint64_t carry = 0;
        for (const auto& pi : constants::p2) {
            const auto i = &pi - &constants::p2[0];
            const auto ai = a[i];
            const auto bi = ai - pi - carry;
            b[i] = bi;
            carry = (pi & ~ai | (pi | ~ai) & bi) >> 63ull;
        }
        carry = carry & ~head;

        // if b is negative, then return a.
        // else return b.
        carry = -carry;
        const auto ncarry = ~carry;
        for (auto i = 0; i < 4; i++) {
            a[i] = (a[i] & carry) | (b[i] & ncarry);
        }
    }

    void gfp_neg(gfp& c, const gfp& a){
        uint64_t carry = 0;
        for (const auto& pi : constants::p2) {
            const auto i = &pi - &constants::p2[0];
            const auto ai = a[i];
            const auto ci = pi - ai - carry;
            c[i] = ci;
            carry = (ai & ~pi | (ai | ~pi) & ci) >> 63ull;
        }
        gfp_carry(c, 0);
    }

    void gfp_add(gfp& c, const gfp& a, const gfp& b){
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

    void gfp_sub(gfp& c, const gfp& a, const gfp& b){
        uint64_t carry = 0;
        gfp t{};
        for (const auto& pi : constants::p2) {
            const auto i = &pi - &constants::p2[0];
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

    static void mul(const gfp& a, const gfp& b, uint64_array_8_t& rc){
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
    }

    static void half_mul(const uint64_array_4_t& a, const uint64_array_4_t& b, uint64_array_4_t& rc) {
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
    }

    void gfp_mul(gfp& c, const gfp& a, const gfp& b) {
        uint64_array_8_t array8_t1{};
        mul(a, b, array8_t1);

        uint64_array_4_t array4_m{};
        const uint64_array_4_t array4_1 = {array8_t1[0], array8_t1[1], array8_t1[2], array8_t1[3]};
        half_mul(array4_1, constants::np, array4_m);

        uint64_array_8_t array8_t2{};
        mul(array4_m, constants::p2, array8_t2);

        uint64_t carry = 0;
        uint64_array_8_t array8_zi{};
        for (const auto& t1_i : array8_t1) {
            const auto i = &t1_i - &array8_t1[0];
            const auto t2_i = array8_t2[i];
            const auto zi = t1_i + t2_i + carry;
            array8_zi[i] = zi;
            carry = (t1_i & t2_i | (t1_i | t2_i) & ~zi) >> 63ull;
        }

        c = {array8_zi[4], array8_zi[5], array8_zi[6], array8_zi[7]};
        gfp_carry(c, carry);
    }
}
