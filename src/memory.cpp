#include <memory/cycle_unroll_benchmark.hpp>

#include <benchmarks/memory.hpp>

namespace benchmarks::memory {

std::vector<cycle_unroll_result> cycle_unroll()
{
    return memory::cycle_unroll_benchmark{}.run();
}

} // namespace benchmarks::memory

