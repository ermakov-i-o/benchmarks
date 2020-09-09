#include <tools/cpu_cache_info.hpp>
#include <tools/data.hpp>
#include <tools/execution.hpp>

#include <memory/cycle_unroll_benchmark.hpp>

#include <utility>

namespace benchmarks::memory {

namespace {

std::size_t get_data_cache_size()
{
    const auto cache_info = tools::get_cache_info();
    return std::accumulate(cache_info.cbegin(), cache_info.cend(), std::size_t{0},
        [](std::size_t value, const auto& cache_info)
        {
            switch(cache_info.type)
            {
            case tools::cache_type::data:
            case tools::cache_type::unified:
                return value + cache_info.size;
            case tools::cache_type::instruction:
                break;
            }
            return value;
        });
}

} // anonymous namespace

cycle_unroll_benchmark::cycle_unroll_benchmark()
{
    const std::size_t chunk_count =
        get_data_cache_size() / max_unroll_count / sizeof(element_type);

    // 8 times larger than the cache size
    // why 8? just because
    count = (chunk_count != 0 ? chunk_count : 1) * max_unroll_count;

    data.reset(new element_type[count]);
    tools::generate_uniform_integers(data.get(), data.get() + count);
}

std::vector<cycle_unroll_result> cycle_unroll_benchmark::run()
{
    return make_result(std::make_index_sequence<max_power_of_two_for_unroll_count>{});
}

template<std::size_t UnrollCount>
std::chrono::duration<float> cycle_unroll_benchmark::get_execution_time()
{
    auto cycle = [this]<size_t... Idx>(std::index_sequence<Idx...>)
    {
        element_type result = 0;
        for(std::size_t i = 0; i < count; i += UnrollCount)
            (result +=...+= data[i + Idx]);
        return result;
    };
    return tools::measure_execution_time(
        [cycle] { return cycle(std::make_index_sequence<UnrollCount>{}); });
}

template<size_t... Idx>
std::vector<cycle_unroll_result> cycle_unroll_benchmark::make_result(std::index_sequence<Idx...>)
{
    return {
        {
            .unroll_count = std::size_t{1} << Idx,
            .speed = count / get_execution_time<std::size_t{1} << Idx>().count() / 1024 / 1024 *
                     sizeof(element_type)
        }...
    };
}

}
