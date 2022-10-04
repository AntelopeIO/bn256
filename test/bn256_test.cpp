#include <bn256.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <memory>
#include <catch2/catch_test_macros.hpp>

using namespace boost::multiprecision::literals;
using namespace boost::multiprecision;

constexpr int test_fail = 1;
constexpr int test_pass = 0;

void print_umarshall_status(bn256::unmarshal_status status) {
    switch(status) {
        case bn256::unmarshal_success:
            std::cout << "unmarshal_success" << std::endl;
            break;
        case bn256::unmarshal_malformed_point:
            std::cout << "unmarshal_malformed_point" << std::endl;
            break;
        case bn256::unmarshal_coordinate_exceeds_modulus:
            std::cout << "unmarshal_coordinate_exceeds_modulus" << std::endl;
            break;
    }
}

TEST_CASE("test g1 marshall", "[g1_marshall]"){
    int512_t k;
    bn256::g1 ga{}, gb{};

    std::tie(k, ga) = bn256::ramdom_g1();
    auto ma = ga.marshal();

    REQUIRE( gb.unmarshal(ma) == bn256::unmarshal_success);
    auto mb = gb.marshal();
    CHECK(ma == mb);




}

// TEST_CASE("test g2 marshall", "[g2_marshall]"){

//     int512_t k;
//     bn256::g2 ga{}, gb{};

//     std::tie(k, ga) = bn256::ramdom_g2();

//     auto ma = ga.marshal();
//     auto status = gb.unmarshal(ma);
//     REQUIRE( gb.unmarshal(ma) == bn256::unmarshal_success);
//     auto mb = gb.marshal();
//     CHECK(ma == mb);
// }

// TEST_CASE("test bilinearity", "[bilinearity]"){
//     int512_t a, b;
//     bn256::g1 p1{}, c1{bn256::curve_gen};
//     bn256::g2 p2{}, c2{bn256::twist_gen};

//     for (auto i = 0; i < 2; ++i) {
//         std::tie(a, p1) = bn256::ramdom_g1();
//         std::tie(b, p2) = bn256::ramdom_g2();
//         bn256::gt e1 = bn256::pair(p1, p2);
//         bn256::gt e2 = bn256::pair(c1, c2);
//         e2.scalar_mult(e2, a);
//         e2.scalar_mult(e2, b);
//         CHECK(e1 == e2);
//     }
// }

// static bn256::g1 test_marshal_g1(const int512_t& base) {
//     bn256::g1 ret, t1;
//     t1.scalar_base_mult(base);
//     auto m1 = t1.marshal();
//     ret.unmarshal(m1);
//     return ret;
// }

// static bn256::g2 test_marshal_g2(const int512_t& base) {
//     bn256::g2 ret, t1;
//     t1.scalar_base_mult(base);
//     auto m1 = t1.marshal();
//     ret.unmarshal(m1);
//     return ret;
// }

// static std::unique_ptr<bn256::uint8_array_32_array_12_t> test_marshal_pair(const bn256::g1& a, const bn256::g2& b, const int512_t& base) {
//     auto pair = bn256::pair(a, b);
//     pair.scalar_mult(pair, base);
//     auto pair_bytes = pair.marshal();
//     auto ptr = std::make_unique<bn256::uint8_array_32_array_12_t>();
//     *ptr = pair_bytes;
//     return ptr;
// }

// TEST_CASE("test tripartite_diffie_hellman", "[tripartite_diffie_hellman]"){

//     constexpr int512_t a = 0xA2422041C5891A948F80F739C3F3B7A32FE306F28ED085741EE155DDFB789C17_cppi512;
//     constexpr int512_t b = a;
//     constexpr int512_t c = a;

//     bn256::g1 pa, pb, pc;
//     bn256::g2 qa, qb, qc;

//     pa = test_marshal_g1(a);
//     qa = test_marshal_g2(a);

//     pb = test_marshal_g1(b);
//     qb = test_marshal_g2(b);

//     pc = test_marshal_g1(b);
//     qc = test_marshal_g2(b);

//     auto k1_bytes = test_marshal_pair(pb, qc, a);
//     auto k2_bytes = test_marshal_pair(pc, qa, b);
//     auto k3_bytes = test_marshal_pair(pa, qb, c);
//     CHECK(*k1_bytes == *k2_bytes);
//     CHECK(*k2_bytes == *k3_bytes);
// }

// TEST_CASE("test g2_self_addition", "[g2_self_addition]"){
//     constexpr int512_t s = 0xA2422041C5891A948F80F739C3F3B7A32FE306F28ED085741EE155DDFB789C17_cppi512;
//     bn256::g2 p;
//     p.scalar_base_mult(s);
//     REQUIRE(p.p_.is_on_curve());

//     p.add(p, p);
//     auto m = p.marshal();
//     CHECK( p.unmarshal(m) == bn256::unmarshal_success);
// }


TEST_CASE("test twist point mul", "[twist_point_mul]"){
	const int512_t k("73391516005847081647776723068736393251206848701235344996976057911204818492439");
	bn256::twist_point tp_gen(bn256::twist_gen);
	bn256::g2 p;
	p.scalar_base_mult(k);

    const bn256::twist_point expected = {
      {
         {0x05ec0a32b82b74d0, 0x79226658a77fa893, 0x127032ff3328c543, 0x1e7d6db7d450f4be},
         {0x8e35b004a08c0b11, 0x791ad513fcc41a41, 0x8842fab42e7eba4b, 0x03da6b5fc89c90ab},
      },{
         {0xd2f8eadf4e38ca86, 0x4b9fed22bbe0dc94, 0x1e29c03a8cf24db7, 0x26ec1edd6d3dac31},
         {0x7c151529d654d2d2, 0x273c7512c431ee15, 0xcc3179ec7d8d6148, 0x15a47d30dd1e78e4},
      },{
         {0xb80ceea187dac34b, 0xc6fe051c511b1d67, 0x1b3efe1172f2c0d2, 0x0b34a4dba3c1edd4},
         {0xfbfc4c2759d29b67, 0x01e318b3356b84c8, 0xf4c1b7f4bd86512a, 0x26f9c677782ad979},
      },
      { bn256::new_gfp(0), bn256::new_gfp(0)}
   };

   CHECK(memcmp(&p, &expected, sizeof(p)) == 0);
}
