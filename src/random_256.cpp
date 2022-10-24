#include "random_256.h"

namespace bn256 {

random_256::random_256() {
   using namespace boost::random;
   uniform_int_distribution_ = uniform_int_distribution<boost::multiprecision::uint256_t>(std::numeric_limits<boost::multiprecision::uint256_t>::min(),
                                                                   std::numeric_limits<boost::multiprecision::uint256_t>::max() / 2);
   std::random_device rd;
   gen_ = generator(rd());
   uniform_int_distribution_(gen_);
}

std::array<uint64_t, 4> random_256::sample() {
    auto k = uniform_int_distribution_(gen_);
    std::array<uint64_t, 4> r{};
    boost::multiprecision::export_bits(k, &r[0], 64, false);
    return r;

}

} // namespace bn256
