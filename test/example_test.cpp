#include <bn256.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <random_256.h>
#include <catch2/catch_test_macros.hpp>

using namespace boost::multiprecision::literals;
using namespace boost::multiprecision;



static bn256::g1 make_scaled_g1(const int512_t& scale_value) {
   bn256::g1 ret;
   ret.scalar_base_mult(scale_value);
   return ret;
}

static bn256::g2 make_scaled_g2(const int512_t& scale_value) {
   bn256::g2 ret;
   ret.scalar_base_mult(scale_value);
   return ret;
}

static bn256::gt make_scaled_pair(const bn256::g1& a, const bn256::g2& b, const int512_t& scale_value) {
   bn256::gt ret = bn256::pair(a, b);
   ret.scalar_mult(ret, scale_value);
   return ret;
}

TEST_CASE("test example pair", "[example]"){

   bn256::random_256 rand;

   auto a = rand.sample();
   auto b = rand.sample();
   auto c = rand.sample();

   auto pa = make_scaled_g1(a);
   auto qa = make_scaled_g2(a);

   auto pb = make_scaled_g1(b);
   auto qb = make_scaled_g2(b);

   auto pc = make_scaled_g1(c);
   auto qc = make_scaled_g2(c);

   auto k1 = make_scaled_pair(pb, qc, a);
   auto k2 = make_scaled_pair(pc, qa, b);
   auto k3 = make_scaled_pair(pa, qb, c);
   CHECK(k1 == k2);
   CHECK(k1 == k3);
}
