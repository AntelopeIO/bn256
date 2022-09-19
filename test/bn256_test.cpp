#include <bn256.h>
#include <boost/multiprecision/cpp_int.hpp>

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

int test_g1_marshall() {
    std::cout << std::endl << "test_g1_marshall:" << std::endl;

    int512_t k;
    bn256::g1 ga{}, gb{};

    std::tie(k, ga) = bn256::ramdom_g1();
    auto ma = ga.marshal();

    auto status = gb.unmarshal(ma);
    if (status != bn256::unmarshal_success){
        print_umarshall_status(status);
        return test_fail;
    }

    auto mb = gb.marshal();
    if (ma != mb) {
        std::cout << "bytes are different" << std::endl;
        return test_fail;
    }

    return test_pass;
}

int test_g2_marshall() {
    std::cout << std::endl << "test_g2_marshall:" << std::endl;

    int512_t k;
    bn256::g2 ga{}, gb{};

    std::tie(k, ga) = bn256::ramdom_g2();

    auto ma = ga.marshal();
    auto status = gb.unmarshal(ma);
    if (status != bn256::unmarshal_success){
        print_umarshall_status(status);
        return test_fail;
    }
    auto mb = gb.marshal();

    if (ma != mb) {
        std::cout << "bytes are different" << std::endl;
        return test_fail;
    }
    return test_pass;
}

int test_bilinearity() {
    std::cout << std::endl << "test_bilinearity:" << std::endl;

    int512_t a, b;
    bn256::g1 p1{}, c1{bn256::curve_gen};
    bn256::g2 p2{}, c2{bn256::twist_gen};

    for (auto i = 0; i < 2; ++i) {
        std::tie(a, p1) = bn256::ramdom_g1();
        std::tie(b, p2) = bn256::ramdom_g2();
        bn256::gt e1 = bn256::pair(p1, p2);
        bn256::gt e2 = bn256::pair(c1, c2);
        e2.scalar_mult(e2, a);
        e2.scalar_mult(e2, b);

        if (e1 != e2) {
            std::cout << "bad pairing result" << std::endl;
            return test_fail;
        }
    }
    return test_pass;
}

static bn256::g1 test_marshal_g1(const int512_t& base) {
    bn256::g1 ret, t1;
    t1.scalar_base_mult(base);
    auto m1 = t1.marshal();
    ret.unmarshal(m1);
    return ret;
}

static bn256::g2 test_marshal_g2(const int512_t& base) {
    bn256::g2 ret, t1;
    t1.scalar_base_mult(base);
    auto m1 = t1.marshal();
    ret.unmarshal(m1);
    return ret;
}

static std::unique_ptr<bn256::uint8_array_32_array_12_t> test_marshal_pair(const bn256::g1& a, const bn256::g2& b, const int512_t& base) {
    auto pair = bn256::pair(a, b);
    pair.scalar_mult(pair, base);
    auto pair_bytes = pair.marshal();
    auto ptr = std::unique_ptr<bn256::uint8_array_32_array_12_t>(new bn256::uint8_array_32_array_12_t);
    *ptr = pair_bytes;
    return ptr;
}

int test_tripartite_diffie_hellman() {
    std::cout << std::endl << "test_tripartite_diffie_hellman:" << std::endl;

    constexpr int512_t a = 0xA2422041C5891A948F80F739C3F3B7A32FE306F28ED085741EE155DDFB789C17_cppi512;
    constexpr int512_t b = a;
    constexpr int512_t c = a;

    bn256::g1 pa, pb, pc;
    bn256::g2 qa, qb, qc;

    pa = test_marshal_g1(a);
    qa = test_marshal_g2(a);

    pb = test_marshal_g1(b);
    qb = test_marshal_g2(b);

    pc = test_marshal_g1(b);
    qc = test_marshal_g2(b);

    auto k1_bytes = test_marshal_pair(pb, qc, a);
    auto k2_bytes = test_marshal_pair(pc, qa, b);
    auto k3_bytes = test_marshal_pair(pa, qb, c);

    if (*k1_bytes != *k2_bytes || *k2_bytes != *k3_bytes) {
        std::cout << "keys didn't agree" << std::endl;
        return test_fail;
    }

    return test_pass;
}

int test_g2_self_addition() {
    std::cout << std::endl << "test_g2_self_addition:" << std::endl;
    constexpr int512_t s = 0xA2422041C5891A948F80F739C3F3B7A32FE306F28ED085741EE155DDFB789C17_cppi512;
    std::unique_ptr<bn256::g1> p(new bn256::g1 {});
    p->scalar_base_mult(s);
    if (!p->p_.is_on_curve()) {
        std::cout << "p isn't on curve" << std::endl;
        return test_fail;
    }
    p->add(*p, *p);
    auto m = p->marshal();
    auto status = p->unmarshal(m);
    if (status != bn256::unmarshal_success) {
        print_umarshall_status(status);
        return test_fail;
    }
    return test_pass;
}

int main() {
    int rc = test_pass;
    rc += test_g1_marshall();
    rc += test_g2_marshall();
    rc += test_bilinearity();
    // rc += test_tripartite_diffie_hellman();
    rc += test_g2_self_addition();
    return rc;
}
