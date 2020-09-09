#ifndef BENCHMARKS_SRC_MEMORY_CYCLE_UNROLL_BENCHMARK_HEADER
#define BENCHMARKS_SRC_MEMORY_CYCLE_UNROLL_BENCHMARK_HEADER

#include <benchmarks/memory.hpp>

#include <chrono>
#include <memory>
#include <utility>

namespace benchmarks::memory {

class cycle_unroll_benchmark
{
public:
    cycle_unroll_benchmark();

    std::vector<cycle_unroll_result> run();

private:
    using element_type = std::uint64_t;

    template<std::size_t UnrollCount>
    std::chrono::duration<float> get_execution_time();

    template<std::size_t... Idx>
    std::vector<cycle_unroll_result> make_result(std::index_sequence<Idx...>);

private:
    static constexpr std::size_t max_power_of_two_for_unroll_count = 11;
    static constexpr std::size_t max_unroll_count =
        std::size_t{1} << max_power_of_two_for_unroll_count;

    std::unique_ptr<element_type[]> data;
    std::size_t count;
};

}

#endif //BENCHMARKS_SRC_MEMORY_CYCLE_UNROLL_BENCHMARK_HEADER
