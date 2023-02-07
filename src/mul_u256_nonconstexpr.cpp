#define BN256_MUL_U256_IMPL_ONLY
#include "bitint_arithmetic.h"
// should compiled without -mbmi2 option

namespace bn256 {

void full_mul_u256_bmi2(const uint64_t* a, const uint64_t* b, uint64_t* c);
void half_mul_u256_bmi2(const uint64_t* a, const uint64_t* b, uint64_t* c);


typedef void (*mul_u256_t)(const uint64_t* a, const uint64_t* b, uint64_t* c);

extern "C" mul_u256_t resolve_full_mul_u256() {
   __builtin_cpu_init();
   if (__builtin_cpu_supports("bmi2"))
      return bn256::full_mul_u256_bmi2;
   return bn256::full_mul_u256_impl;
}

void full_mul_u256_nonconstexpr(const uint64_t* a, const uint64_t* b, uint64_t* c) __attribute__((ifunc("resolve_full_mul_u256")));


extern "C" mul_u256_t resolve_half_mul_u256() {
   __builtin_cpu_init();
   if (__builtin_cpu_supports("bmi2"))
      return bn256::half_mul_u256_bmi2;
   return bn256::half_mul_u256_impl;
}

void half_mul_u256_nonconstexpr(const uint64_t* a, const uint64_t* b, uint64_t* c) __attribute__((ifunc("resolve_half_mul_u256")));
}
