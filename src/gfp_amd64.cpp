#include "gfp_native.h"

extern "C" {

u256_t gfp_neg(u256_t a) {
   auto r = bn256::native::gfp_neg(bn256::native::bit_cast<bn256::array<uint64_t, 4>>(a));
   return bn256::native::bit_cast<struct u256_t>(r);
}

u256_t gfp_add(u256_t a, u256_t b) {
   auto r = bn256::native::gfp_add(bn256::native::bit_cast<bn256::array<uint64_t, 4>>(a), bn256::native::bit_cast<bn256::array<uint64_t, 4>>(b));
   return bn256::native::bit_cast<struct u256_t>(r);
}

u256_t gfp_sub(u256_t a, u256_t b) {
   auto r = bn256::native::gfp_sub(bn256::native::bit_cast<bn256::array<uint64_t, 4>>(a), bn256::native::bit_cast<bn256::array<uint64_t, 4>>(b));
   return bn256::native::bit_cast<struct u256_t>(r);
}
}