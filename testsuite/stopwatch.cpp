#include <gold/stopwatch>
#include <gold/bits/assume.hpp>
#include <thread>
#include <iostream>

// ranges::begin, ranges::end           -> <bits/ranges_base.h>
// ranges::iter_swap, ranges::iter_move -> <bits/iterator_concepts.h>

int main() {
    using MicroSeconds = std::chrono::microseconds;
    using MilliSeconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;

    // example:
    {
        std::cout << "test number 1" << '\n';
        gold::stopwatch stopwatch { gold::stopwatch::start_defer };
        gold::assume(!stopwatch.has_started());
        gold::assume(!stopwatch.is_running());
        gold::assume(std::chrono::duration_cast<MicroSeconds>(stopwatch.elapsed()) == MicroSeconds(0));

        stopwatch.start();
        if (!stopwatch.has_started() || !stopwatch.is_running())
            return -1;

        // which means stopwatch has started and is running
        std::this_thread::sleep_for(MilliSeconds(500));
//        gold::assume(std::chrono::duration_cast<MilliSeconds>(stopwatch.elapsed()) >= MilliSeconds(100));
//        gold::assume(stopwatch.has_started());
//        gold::assume(stopwatch.is_running());

        stopwatch.stop();
        if (!stopwatch.has_started() || stopwatch.is_running())
            return -1;

        // which means stopwatch has started and currently not running
        std::cout << "elapsed time: " << std::chrono::duration_cast<MilliSeconds>(stopwatch.elapsed()) << '\n';

        // reset the stopwatch
        auto total_time = stopwatch.reset();
        if (stopwatch.has_started() || stopwatch.is_running() || stopwatch.elapsed() != MilliSeconds(0))
            return -1;

        std::cout << "elapsed time: " << std::chrono::duration_cast<MilliSeconds>(total_time) << '\n';

//        gold::assume(std::chrono::duration_cast<MilliSeconds>(stopwatch.elapsed()) >= MilliSeconds(100));
//        gold::assume(!stopwatch.has_started());
//        gold::assume(stopwatch.is_running());
    }

    {
        std::cout << "test number 2" << '\n';
        gold::stopwatch stopwatch { gold::stopwatch::start_eager };
        std::this_thread::sleep_for(MilliSeconds(500));
        stopwatch.stop();
        if (stopwatch.is_running())
            return -1;

        std::cout << "elapsed time: " << std::chrono::duration_cast<MilliSeconds>(stopwatch.elapsed()) << '\n';
        stopwatch.resume();
        std::this_thread::sleep_for(MilliSeconds(500));
        stopwatch.stop();
        if (stopwatch.is_running())
            return -1;

        std::this_thread::sleep_for(MilliSeconds(500));

        std::cout << "total elapsed time: " << std::chrono::duration_cast<MilliSeconds>(stopwatch.elapsed()) << '\n';

        stopwatch.restart();

        std::this_thread::sleep_for(MilliSeconds(750));

        std::cout << "total elapsed time after restart: " << std::chrono::duration_cast<MilliSeconds>(stopwatch.elapsed()) << '\n';
    }

    // API
    // + gold::stopwatch
    // + gold::stopwatch::start
    // + gold::stopwatch::restart
    // + gold::stopwatch::stop
    // + gold::stopwatch::resume
    // + gold::stopwatch::reset
    // + gold::stopwatch::elapsed
}
