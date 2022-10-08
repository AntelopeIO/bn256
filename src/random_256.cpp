#include <random_256.h>

namespace bn256 {

random_256::random_256() {
   using namespace boost::random;
   uniform_int_distribution_ = uniform_int_distribution<uint256_t>(std::numeric_limits<uint256_t>::min(),
                                                                   std::numeric_limits<uint256_t>::max() / 2);
   std::random_device rd;
   gen_ = generator(rd());
   uniform_int_distribution_(gen_);
}

uint256_t random_256::sample() { return uniform_int_distribution_(gen_); }

} // namespace bn256
