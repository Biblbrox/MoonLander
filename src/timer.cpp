#include <SDL_timer.h>
#include "timer.hpp"

utils::Timer::Timer() : m_startTicks(0), m_pausedTicks(0),
                        m_paused(false), m_started(false)
{

}

void utils::Timer::start()
{
    m_started = true;
    m_paused = false;

    m_startTicks = SDL_GetTicks();
    m_pausedTicks = 0;
}

void utils::Timer::stop()
{
    m_started = false;
    m_paused = false;

    m_startTicks = 0;
    m_pausedTicks = 0;
}

void utils::Timer::pause()
{
    if (m_started && !m_paused) {
        m_paused = true;

        m_pausedTicks = SDL_GetTicks() - m_startTicks;
        m_startTicks = 0;
    }
}

void utils::Timer::unpause()
{
    if (m_started && m_paused) {
        m_paused = false;

        m_startTicks = SDL_GetTicks() - m_pausedTicks;
    }
}

Uint32 utils::Timer::getTicks() const
{
    Uint32 time = 0;

    if (m_started)
        time = m_paused ? m_pausedTicks : (SDL_GetTicks() - m_startTicks);

    return time;
}

bool utils::Timer::isStarted() const
{
    return m_started;
}

bool utils::Timer::isPaused() const
{
    return m_paused && m_started;
}
