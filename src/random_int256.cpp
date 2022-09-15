#include <random_int256.h>
#include <constants.h>

namespace bn256 {
    random_int256::random_int256() {
        ur_ = boost::random::uniform_int_distribution<uint256_t>(
                std::numeric_limits<uint256_t>::min(),
                std::numeric_limits<uint256_t>::max()/2);
        std::random_device rd;
        gen_ = generator(rd());
        ur_(gen_);
    }

    uint256_t random_int256::sample() {
        uint256_t ret = ur_(gen_);
        return ret;
    }

}
