#include "random_256.h"
#include <openssl/rand.h>

namespace bn256 {

std::array<uint64_t, 4>  random_256() {
   // the random number should be a 255 bits integer
   std::array<uint64_t, 4> result;
   do {
      RAND_bytes( reinterpret_cast<unsigned char*>(&result),  sizeof(uint64_t)*result.size());
   } while (static_cast<int64_t>(result.back()) < 0);
   return result;
}


} // namespace bn256
