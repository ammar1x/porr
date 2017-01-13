//
// Created by Ammar on 12/24/16.
//

#ifndef PORR_STOPWATCH_H
#define PORR_STOPWATCH_H

#include <chrono>
#include <tuple>

/**
 * Src: https://codereview.stackexchange.com/questions/48872/measuring-execution-time-in-c
 *
 */
template<typename TimeT = std::chrono::microseconds,
        typename ClockT = std::chrono::high_resolution_clock,
        typename DurationT=double>
class Stopwatch
{
private:
    std::chrono::time_point<ClockT> _start, _end;
public:
    Stopwatch() { start(); }
    void start() { _start = _end = ClockT::now(); }
    DurationT stop() { _end = ClockT::now(); return elapsed();}
    DurationT elapsed() {
        std::chrono::time_point<ClockT> _end = ClockT::now();
        auto delta = std::chrono::duration_cast<TimeT>(_end-_start);
        return delta.count();
    }
};

template <class Result, class F, typename ...Args>
tuple<Result, double> timeit(F func, Args&& ...args) {
    Stopwatch<> sw;
    Result res = func(forward<Args>(args)...);
    return std::make_tuple(res, sw.elapsed());
}

#endif //PORR_STOPWATCH_H
