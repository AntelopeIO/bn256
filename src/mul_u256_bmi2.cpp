#define BN256_MUL_U256_IMPL_ONLY
#include "bitint_arithmetic.h"
// should compiled with -mbmi2 option

namespace bn256 {
void full_mul_u256_bmi2(const uint64_t* a, const uint64_t* b, uint64_t* c) noexcept{
    return full_mul_u256_impl(a, b, c);
}

void half_mul_u256_bmi2(const uint64_t* a, const uint64_t* b, uint64_t* c) noexcept {
    return half_mul_u256_impl(a, b, c);
}
}
