#include <bn256.h>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <iomanip>

static bn256::g1 make_scaled_g1(bn256::uint255_t scalar) {
   return bn256::g1::scalar_base_mult(scalar);
}

static bn256::g2 make_scaled_g2(bn256::uint255_t scalar) {
   return bn256::g2::scalar_base_mult(scalar);
}

static bn256::gt make_scaled_gt(const bn256::gt& a, bn256::uint255_t scalar) {
   return a.scalar_mult(scalar);
}

std::vector<uint8_t> encode_pairs(const std::vector<bn256::g1>& g1_vec, std::vector<bn256::g2>& g2_vec ) {
   const int num_pairs = g1_vec.size();
   std::vector<uint8_t> result;
   result.resize( (64+128) * num_pairs);
   uint8_t* data = (uint8_t*) result.data();
   for (int i = 0; i <num_pairs; ++i) {
      g1_vec[i].marshal({data, 64});
      data += 64;
      g2_vec[i].marshal({data, 128});
      data += 128;
   }
   return result;
}

TEST_CASE("test_pairings", "[main]") {
   // n1 =  21888242871839275222246405745257275088548364400416034343698204186575808495616
   constexpr bn256::uint255_t n1 = {0x43e1f593f0000000, 0x2833e84879b97091, 0xb85045b68181585d, 0x30644e72e131a029 };
   auto a1 = make_scaled_g1({ 1, 0, 0, 0});
   auto a2 = make_scaled_g1({ 2, 0, 0, 0});
   auto a37 = make_scaled_g1({ 37, 0, 0, 0});
   auto an1 = make_scaled_g1(n1);

   auto b0 = make_scaled_g2({ 0, 0, 0, 0});
   auto b1 = make_scaled_g2({ 1, 0, 0, 0});
   auto b2 = make_scaled_g2({ 2, 0, 0, 0});
   auto b27 = make_scaled_g2({ 27, 0, 0, 0});
   auto b999 = make_scaled_g2({ 999, 0, 0, 0});
   auto bn1 = make_scaled_g2(n1);

   auto p1 = bn256::pair(a1, b1);
   auto pn1 = bn256::pair(a1, bn1);
   auto np1 = bn256::pair(an1, b1);

   CHECK(pn1 == np1);

   CHECK(pn1.string() == np1.string()); // Pairing mismatch

   std::vector<bn256::g1> g1_vec = {a1, an1};
   std::vector<bn256::g2> g2_vec = {b1, b1};

   CHECK(bn256::pairing_check(g1_vec, g2_vec)); // MultiAte check gave false negative!

   auto yield = []{};

   CHECK(bn256::pairing_check(encode_pairs(g1_vec, g2_vec), yield) == 1);

   auto p0 = bn256::gt{};
   p0 = p1.add(pn1);
   auto p0_2 = bn256::pair(a1, b0);
   CHECK (p0.string() == p0_2.string()); // Pairing mismatch

   auto p0_3 = make_scaled_gt(p1, { n1[0]+1, n1[1], n1[2], n1[3]});
   CHECK (p0.string() == p0_3.string()); // Pairing mismatch

   auto p2 = bn256::pair(a2, b1);
   auto p2_2 = bn256::pair(a1, b2);
   auto p2_3 = make_scaled_gt(p1, { 2, 0, 0, 0});
   CHECK(p2.string() == p2_2.string()); // Pairing mismatch
   CHECK(p2.string() == p2_3.string()); // Pairing mismatch

   CHECK(p2.string() != p1.string()); // Pairing is degenerate!

   g1_vec = {a1, a1};
   g2_vec = {b1, b1};
   CHECK(!bn256::pairing_check(g1_vec, g2_vec)) ; // MultiAte check gave false positive!
   CHECK(bn256::pairing_check(encode_pairs(g1_vec, g2_vec), yield) == 0);

   auto p999 = bn256::pair(a37, b27);
   auto p999_2 = bn256::pair(a1, b999);
   CHECK(p999.string() == p999_2.string()); // Pairing mismatch
}

TEST_CASE("test_g1_add", "[main]") {
   constexpr bn256::uint255_t n1 = {0x43e1f593f0000000, 0x2833e84879b97091, 0xb85045b68181585d, 0x30644e72e131a029 };
   auto an1 = make_scaled_g1(n1);
   auto a37 = make_scaled_g1({ 37, 0, 0, 0});

   std::array<uint8_t, 64> r;
   CHECK(0 == bn256::g1_add(an1.marshal(), a37.marshal(), r));

   uint64_t rr[8] = {0x000000009f85c51a, 0x000000003cc4ec99, 0x000000008934662f, 0x00000000ddae71c5, 0x0000000010d94d23, 0x00000000a5baafba, 0x00000000c916998f, 0x00000000d49c6452};
   CHECK( memcmp(rr, &r, sizeof(64)) == 0);

   const uint64_t invalid_data[8] = {UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX};
   CHECK(-1 == bn256::g1_add({(const uint8_t*)invalid_data , 64}, {(const uint8_t*)invalid_data , 64}, r));

}

TEST_CASE("test_g1_scalar_mul", "[main]") {
   constexpr bn256::uint255_t n1 = {0x43e1f593f0000000, 0x2833e84879b97091, 0xb85045b68181585d, 0x30644e72e131a029 };
   auto an1 = make_scaled_g1(n1);
   const bn256::uint255_t k = {0x1ee155ddfb789c17, 0x2fe306f28ed08574, 0x8f80f739c3f3b7a3, 0x72422041c5891a94};

   std::array<uint8_t, 64> r;
   CHECK(0 == bn256::g1_scalar_mul(an1.marshal(), {(const uint8_t*)&k, 32}, r));

   uint64_t rr[8] = {0x5bf039225dc43b2e,0xe598cd228b2ef414,0x878c69dbbfd92c77,0x8a92d653746b3823,0x9f9cc64b7152c606,0x89433035500f769e,0xebcc794b9937e535,0xd1b6b722ffdf9f1c};
   CHECK(memcmp(rr, r.data(), 64) == 0);


   const uint64_t invalid_data[8] = {UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX};
   CHECK(-1 == bn256::g1_scalar_mul({(const uint8_t*)invalid_data , 64}, {(const uint8_t*)invalid_data , 64}, r));
}
