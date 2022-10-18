#include "gfp_native.h"

extern "C" {

struct u256_t  gfp_mul(struct u256_t a, struct u256_t b) {
    auto r = bn256::native::gfp_mul(bn256::native::bit_cast<bn256::array<uint64_t, 4>>(a), bn256::native::bit_cast<bn256::array<uint64_t, 4>>(b));
    return bn256::native::bit_cast<struct u256_t>(r);
}
}