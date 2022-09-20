#include <bn256.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <random_256.h>

using namespace boost::multiprecision::literals;
using namespace boost::multiprecision;

constexpr int test_fail = 1;
constexpr int test_pass = 0;

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

int test_example_pair() {
    int rc = test_pass;

    std::cout << std::endl << "test_example_pair:" << std::endl;

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

    if (k1 != k2) {
        std::cout << k1.string() << std::endl;
        std::cout << k2.string() << std::endl;
        std::cout << "k1 != k2" << std::endl;
        rc = test_fail;
    }
    if (k1 != k3) {
        std::cout << k1.string() << std::endl;
        std::cout << k3.string() << std::endl;
        std::cout << "k1 != k3" << std::endl;
        rc = test_fail;
    }

    return rc;
}

int main() {
    int rc = test_pass;
    rc += test_example_pair();
    return rc;
}
