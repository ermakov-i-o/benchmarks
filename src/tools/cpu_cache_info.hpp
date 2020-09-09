#ifndef BENCHMARKS_TOOLS_CPU_CACHE_INFO_HEADER
#define BENCHMARKS_TOOLS_CPU_CACHE_INFO_HEADER

#include <cstdint>
#include <vector>

namespace benchmarks::tools {

enum class cache_type
{
    instruction,
    data,
    unified
};

struct cpu_cache_info
{
    std::size_t size;
    cache_type type;
    std::uint16_t level;
};

std::vector<cpu_cache_info> get_cache_info();

} // namespace benchmarks::tools


#endif //BENCHMARKS_TOOLS_CPU_CACHE_INFO_HEADER
