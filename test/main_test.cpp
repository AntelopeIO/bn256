#include <bn256.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision::literals;
using namespace boost::multiprecision;

constexpr int test_fail = 1;
constexpr int test_pass = 0;

static bn256::g1 make_scaled_g1(const std::string& scale_string) {
    bn256::g1 ret;
    int512_t scale_value(scale_string);
    ret.scalar_base_mult(scale_value);
    return ret;
}

static bn256::g2 make_scaled_g2(const std::string& scale_string) {
    bn256::g2 ret;
    int512_t scale_value(scale_string);
    ret.scalar_base_mult(scale_value);
    return ret;
}

static bn256::gt make_scaled_gt(const bn256::gt& a, const std::string& scale_string) {
    bn256::gt ret;
    int512_t scale_value(scale_string);
    ret.scalar_mult(a, scale_value);
    return ret;
}

int test_pairings() {
    std::cout << std::endl << "test_pairings:" << std::endl;
    int rc = test_pass;

    constexpr char n1[] = "21888242871839275222246405745257275088548364400416034343698204186575808495616";
    auto a1 = make_scaled_g1("1");
    auto a2 = make_scaled_g1("2");
    auto a37 = make_scaled_g1("37");
    auto an1 = make_scaled_g1(n1);

    auto b0 = make_scaled_g2("0");
    auto b1 = make_scaled_g2("1");
    auto b2 = make_scaled_g2("2");
    auto b27 = make_scaled_g2("27");
    auto b999 = make_scaled_g2("999");
    auto bn1 = make_scaled_g2(n1);

    auto p1 = bn256::pair(a1, b1);
    auto pn1 = bn256::pair(a1, bn1);
    auto np1 = bn256::pair(an1, b1);

    std::cout << an1.string() << std::endl;
    std::cout << bn1.string() << std::endl;
    std::cout << pn1.string() << std::endl;
    std::cout << np1.string() << std::endl;

    if (pn1.string() != np1.string()) {
        rc = test_fail;
        std::cout << "Pairing mismatch" << std::endl;
    }

    std::vector<bn256::g1> g1_vec = {a1, an1};
    std::vector<bn256::g2> g2_vec = {b1, b1};
    if (!bn256::pairing_check(g1_vec, g2_vec)) {
        rc = test_fail;
        std::cout << "MultiAte check gave false negative!" << std::endl;
    }

    auto p0 = bn256::gt{};
    p0.add(p1, pn1);
    auto p0_2 = bn256::pair(a1, b0);
    if (p0.string() != p0_2.string()) {
        rc = test_fail;
        std::cout << p0.string() << std::endl;
        std::cout << p0_2.string() << std::endl;
        std::cout << "Pairing mismatch" << std::endl;
    }

    auto p0_3 = make_scaled_gt(p1, "21888242871839275222246405745257275088548364400416034343698204186575808495617");
    if (p0.string() != p0_3.string()) {
        rc = test_fail;
        std::cout << p0.string() << std::endl;
        std::cout << p0_3.string() << std::endl;
        std::cout << "Pairing mismatch" << std::endl;
    }

    auto p2 = bn256::pair(a2, b1);
    auto p2_2 = bn256::pair(a1, b2);
    auto p2_3 = make_scaled_gt(p1, "2");
    if (p2.string() != p2_2.string()) {
        rc = test_fail;
        std::cout << p2.string() << std::endl;
        std::cout << p2_2.string() << std::endl;
        std::cout << "Pairing mismatch" << std::endl;
    }
    if (p2.string() != p2_3.string()) {
        rc = test_fail;
        std::cout << p2.string() << std::endl;
        std::cout << p2_3.string() << std::endl;
        std::cout << "Pairing mismatch" << std::endl;
    }

    if (p2.string() == p1.string()) {
        rc = test_fail;
        std::cout << p2.string() << std::endl;
        std::cout << p1.string() << std::endl;
        std::cout << "Pairing is degenerate!" << std::endl;
    }

    g1_vec = {a1, a1};
    g2_vec = {b1, b1};
    if (bn256::pairing_check(g1_vec, g2_vec)) {
        rc = test_fail;
        std::cout << "MultiAte check gave false positive!" << std::endl;
    }

    auto p999 = bn256::pair(a37, b27);
    auto p999_2 = bn256::pair(a1, b999);
    if (p999.string() != p999_2.string()) {
        rc = test_fail;
        std::cout << p999.string() << std::endl;
        std::cout << p999_2.string() << std::endl;
        std::cout << "Pairing mismatch" << std::endl;
    }

    return rc;
}

int main() {
    int rc = test_pass;
    rc += test_pairings();
    return rc;
}
