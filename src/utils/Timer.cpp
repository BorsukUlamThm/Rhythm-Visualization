#include "Timer.h"

void Timer::start()
{
    if (state == RUNNING)
    { return; }

    start_time = clock::now();

    state = RUNNING;
}

void Timer::stop()
{
    if (state == STOPPED)
    { return; }

    time_point now = clock::now();
    duration time_span = std::chrono::duration_cast<duration>(now - start_time);
    buffer += time_span.count();

    state = STOPPED;
}

void Timer::start_or_stop()
{
    if (state == RUNNING)
    {
        stop();
    }
    else
    {
        start();
    }
}

void Timer::reset()
{
    start_time = clock::now();
    buffer = 0;
}

Timer::State Timer::read_state()
{
    return state;
}

Timer::time_t Timer::read()
{
    if (state == STOPPED)
    {
        return buffer;
    }

    time_point now = clock::now();
    duration time_span = std::chrono::duration_cast<duration>(now - start_time);
    return buffer + time_span.count();
}
