#ifndef BENCHMARKS_SRC_TOOLS_EXECUTION_HEADER
#define BENCHMARKS_SRC_TOOLS_EXECUTION_HEADER

#include <tools/stopwatch.hpp>

#include <algorithm>
#include <type_traits>

namespace benchmarks::tools {

template<class F>
stopwatch::duration measure_execution_time(F&& func)
    requires (!std::is_same_v<std::invoke_result_t<F&&>, void>)
{
    constexpr std::size_t attempts_count = 12;
    auto execution_time = stopwatch::duration::max();
    for(std::size_t attempt = 0; attempt < attempts_count; ++attempt)
    {
        stopwatch timer;
        volatile auto dummy = func();
        execution_time = std::min(execution_time, timer.get_duration());
    }
    return execution_time;
}

}

#endif //BENCHMARKS_SRC_TOOLS_EXECUTION_HEADER
