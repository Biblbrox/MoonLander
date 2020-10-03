#ifndef MOONLANDER_TIMER_HPP
#define MOONLANDER_TIMER_HPP

#include <SDL_types.h>
#include <vector>

namespace utils
{
    class Timer
    {
    public:
        Timer() noexcept;

        void start() noexcept;
        void stop() noexcept;
        void pause() noexcept;
        void unpause() noexcept;

        Uint32 getTicks() const noexcept;

        bool isStarted() const noexcept;
        bool isPaused() const noexcept;

    private:
        Uint32 m_startTicks;
        Uint32 m_pausedTicks;

        bool m_paused;
        bool m_started;
    };
}


#endif //MOONLANDER_TIMER_HPP
