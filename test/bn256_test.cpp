#include <bn256.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <memory>
#include <catch2/catch_test_macros.hpp>
#include "twist.h"
#include "curve.h"



std::array<uint64_t, 4> to_u256(boost::multiprecision::int512_t x) {
    uint64_t r[8]= {};
    boost::multiprecision::export_bits(x, &r[0], 64, false);
    return {r[0], r[1], r[2], r[3]};
}

TEST_CASE("test g1 marshall", "[bn256]"){
    auto [_, ga] = bn256::ramdom_g1();
    std::array<uint8_t, 64> ma, mb;
    ga.marshal(ma);

    bn256::g1 gb{};

    REQUIRE( gb.unmarshal(ma) == std::error_code{});

    gb.marshal(mb);
    CHECK(ma == mb); // "fail if bytes are different"
}

TEST_CASE("test g2 marshall", "[bn256]"){
    auto [_, ga] = bn256::ramdom_g2();
    std::array<uint8_t, 128> ma, mb;
    ga.marshal(ma);

    bn256::g2 gb{};
    REQUIRE( gb.unmarshal(ma) ==  std::error_code{});
    gb.marshal(mb);
    CHECK(ma == mb); // "fail if bytes are different"
}

TEST_CASE("test bilinearity", "[bn256]"){
    bn256::g1 c1{bn256::curve_gen};
    bn256::g2 c2{bn256::twist_gen};

    for (auto i = 0U; i < 2; ++i) {
        auto [a, p1] = bn256::ramdom_g1();
        auto [b, p2] = bn256::ramdom_g2();
        bn256::gt e1 = bn256::pair(p1, p2);

        bn256::gt e2 = bn256::pair(c1, c2);
        e2 = e2.scalar_mult(a);
        e2 = e2.scalar_mult(b);
        CHECK(e1 == e2); // "fail if keys didn't agree"
    }
}

TEST_CASE("test tripartite_diffie_hellman", "[bn256]"){

    std::array<uint64_t, 4> a = {(uint64_t)rand(), 0, 0, 0};
    std::array<uint64_t, 4> b = {(uint64_t)rand(), 0, 0, 0};
    std::array<uint64_t, 4> c = {(uint64_t)rand(), 0, 0, 0};

    bn256::g1 pa, pb, pc;
    bn256::g2 qa, qb, qc;

    REQUIRE(pa.unmarshal(bn256::g1::scalar_base_mult(a).marshal()) == std::error_code{});
    REQUIRE(qa.unmarshal(bn256::g2::scalar_base_mult(a).marshal()) == std::error_code{});
    REQUIRE(pb.unmarshal(bn256::g1::scalar_base_mult(b).marshal()) == std::error_code{});
    REQUIRE(qb.unmarshal(bn256::g2::scalar_base_mult(b).marshal()) == std::error_code{});
    REQUIRE(pc.unmarshal(bn256::g1::scalar_base_mult(c).marshal()) == std::error_code{});
    REQUIRE(qc.unmarshal(bn256::g2::scalar_base_mult(c).marshal()) == std::error_code{});

    auto k1_bytes = bn256::pair(pb, qc).scalar_mult(a).marshal();
    auto k2_bytes = bn256::pair(pc, qa).scalar_mult(b).marshal();
    auto k3_bytes = bn256::pair(pa, qb).scalar_mult(c).marshal();

    CHECK(k1_bytes == k2_bytes); // "fail if keys didn't agree"
    CHECK(k2_bytes == k3_bytes);
}

TEST_CASE("test g2_self_addition", "[bn256]"){
    std::array<uint64_t, 4> s = {(uint64_t)rand(), 0, 0, 0};
    bn256::g2 p = bn256::g2::scalar_base_mult(s);
    REQUIRE(p.p().is_on_curve()); // "fail if p isn't on curve"

    p = p.add(p);
    auto m = p.marshal();
    CHECK( p.unmarshal(m) == std::error_code{}); // "fail if unmarshalling p.Add(p, p) ∉ G₂"
}


TEST_CASE("test twist point mul", "[bn256]"){
	const boost::multiprecision::int512_t k("73391516005847081647776723068736393251206848701235344996976057911204818492439");
	bn256::g2 p = bn256::g2::scalar_base_mult(to_u256(k));

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

   CHECK(p.p() == expected);
}
