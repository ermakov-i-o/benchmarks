#ifndef BENCHMARKS_SRC_TOOLS_STOPWATCH_HEADER
#define BENCHMARKS_SRC_TOOLS_STOPWATCH_HEADER

#include <chrono>

namespace benchmarks::tools {

class stopwatch
{
public:
    using clock = std::chrono::high_resolution_clock;
    using duration = clock::duration;

private:
    using time_point = clock::time_point;

    time_point start;

public:
    stopwatch() : start{clock::now()} {}

    [[nodiscard]]
    duration get_duration() const noexcept
    {
        return clock::now() - start;
    }
};

} // namespace benchmarks::tools

#endif //BENCHMARKS_SRC_TOOLS_STOPWATCH_HEADER
