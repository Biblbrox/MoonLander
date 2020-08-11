#include <SDL_timer.h>
#include "timer.hpp"

Utils::Timer::Timer() : startTicks(0), pausedTicks(0),
                        paused(false), started(false)
{

}

void Utils::Timer::start()
{
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void Utils::Timer::stop()
{
    started = false;
    paused = false;

    startTicks = 0;
    pausedTicks = 0;
}

void Utils::Timer::pause()
{
    if (started && !paused) {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Utils::Timer::unpause()
{
    if (started && paused) {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;
    }
}

Uint32 Utils::Timer::getTicks() const
{
    Uint32 time = 0;

    if (started)
        time = paused ? pausedTicks : (SDL_GetTicks() - startTicks);

    return time;
}

bool Utils::Timer::isStarted() const
{
    return started;
}

bool Utils::Timer::isPaused() const
{
    return paused && started;
}
