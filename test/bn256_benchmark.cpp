

#include "bn256.h"

#include <chrono>


template <typename Fun>
void benchmark(const char* metric, int count, Fun&& fun) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int i = 0; i < count; ++i)
        fun();

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    std::cout << metric << "\t" << elapsed/count << " ns\n";
}


int main(int, const char**) {

    benchmark("g1::scaler_base_multi", 1000, []() {
        bn256::g1::scalar_base_mult(int512_t("17482507071749278111455282041915610272829864719113987536544577255487650163890"));
    });

    benchmark("g2::scaler_base_multi", 1000, []() {
        bn256::g2::scalar_base_mult(int512_t("14506523411943850241455301787384885005987154472366374992538170185465884650319"));
    });

    benchmark("bn256 pair", 1000, []() { bn256::pair(bn256::g1::curve_gen, bn256::g2::twist_gen); });

    return 0;
}
