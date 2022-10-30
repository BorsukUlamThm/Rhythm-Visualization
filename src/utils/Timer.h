#pragma once

#include <chrono>


class Timer
{
public:
    typedef std::chrono::steady_clock clock;
    typedef clock::time_point time_point;
    typedef double time_t;
    typedef std::chrono::duration<time_t> duration;

protected:
    enum State
    {
        RUNNING,
        STOPPED
    };

    State state = STOPPED;
    time_point start_time;
    time_t buffer = 0;

public:
    Timer() = default;

    void start();
    void stop();
    void start_or_stop();
    void reset();

    State read_state();
    time_t read();
};

