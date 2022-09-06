#include <lattice.h>

using boost::multiprecision::int128_t;

namespace bn256 {

    // decompose takes a scalar mod order as input and finds a short,
    // positive decomposition of it wrt to the lattice basis.
    std::vector<int128_t> lattice::decompose(const int128_t& k) const {
        auto n = inverse_.size();
        std::vector<int128_t> c(n);

        for (std::size_t i = 0; i < n; i++) {
            c[i] = k * inverse_[i];
            round(c[i], det_);
        }

        // Transform vectors according to c and subtract <k,0,0,...>.
        std::vector<int128_t> out(n);
        int128_t temp;

        for (std::size_t i = 0; i < n; i++) {

            out[i] = 0;

            for (std::size_t j = 0; j < n; j++) {
                boost::multiprecision::multiply(temp, c[j], vectors_[j][i]);
                boost::multiprecision::add(out[i], out[i], temp);
            }

            -out[i];
            boost::multiprecision::add(out[i], out[i], vectors_[0][i]);
            boost::multiprecision::add(out[i], out[i], vectors_[0][i]);
        }
        boost::multiprecision::add(out[0], out[0], k);

        return out;
    }

    std::size_t bitlen(const int128_t& value) {
        std::size_t len = 0;
        auto x = abs(value);
        while(x) {
            len++;
            x >>= 1;
        }
        return len;
    }

    std::vector<uint8_t> lattice::multi(const int128_t& scalar) const  {
        std::vector<int128_t> decomp = decompose(scalar);

        std::size_t maxlen = 0;
        for (const auto& x : decomp) {

            const auto len = bitlen(x);
            if (len > maxlen) {
                maxlen = len;
            }
        }

        std::vector<uint8_t> out(maxlen);
        for (auto& x : decomp) {
            const auto j = &x - &decomp[0];
            for (std::size_t i = 0; i < maxlen; i++) {
                if (boost::multiprecision::bit_test(x, i)) {
                    out[i] += (1 << j);
                }
            }
        }

        return out;
    }

    // round sets num to num/denom rounded to the nearest integer.
    void lattice::round(int128_t& num, const int128_t& denom) {
        if (denom != 0) {
            const int128_t half = constants::order >> 1;
            int128_t q, r;
            divide_qr(num, denom, q, r);
            auto compare_result = r.compare(half);
            if (compare_result > 0) {
                num++;
            } else if( compare_result < 0) {
                num--;
            }
        }
    }

}
