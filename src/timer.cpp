#include <SDL_timer.h>
#include "timer.hpp"

utils::Timer::Timer() : startTicks(0), pausedTicks(0),
                        paused(false), started(false)
{

}

void utils::Timer::start()
{
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void utils::Timer::stop()
{
    started = false;
    paused = false;

    startTicks = 0;
    pausedTicks = 0;
}

void utils::Timer::pause()
{
    if (started && !paused) {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void utils::Timer::unpause()
{
    if (started && paused) {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;
    }
}

Uint32 utils::Timer::getTicks() const
{
    Uint32 time = 0;

    if (started)
        time = paused ? pausedTicks : (SDL_GetTicks() - startTicks);

    return time;
}

bool utils::Timer::isStarted() const
{
    return started;
}

bool utils::Timer::isPaused() const
{
    return paused && started;
}
