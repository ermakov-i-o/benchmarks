#ifndef BENCHMARKS_MEMORY_HEADER
#define BENCHMARKS_MEMORY_HEADER

#include <benchmarks/export.hpp>

#include <cstdint>
#include <vector>

namespace benchmarks::memory {

struct cycle_unroll_result
{
    std::size_t unroll_count;
    float speed;
};

BENCHMARKS_EXPORT std::vector<cycle_unroll_result> cycle_unroll();

} // namespace benchmarks::memory

#endif //BENCHMARKS_MEMORY_HEADER
