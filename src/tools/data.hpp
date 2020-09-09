#ifndef BENCHMARKS_SRC_TOOLS_DATA_HEADER
#define BENCHMARKS_SRC_TOOLS_DATA_HEADER

#include <algorithm>
#include <chrono>
#include <iterator>
#include <random>

namespace benchmarks::tools {

template<class FwdIter>
void generate_uniform_integers(FwdIter first, FwdIter last)
{
    using value_type = typename std::iterator_traits<FwdIter>::value_type;
    std::random_device r;
    using clock = std::chrono::system_clock;
    std::seed_seq seed{r(), static_cast<unsigned int>(clock::to_time_t(clock::now()))};
    std::mt19937 engine{seed};
    std::uniform_int_distribution<value_type> distribution;
    std::generate(first, last, [&distribution, &engine] { return distribution(engine); });
}

}

#endif //BENCHMARKS_SRC_TOOLS_DATA_HEADER
