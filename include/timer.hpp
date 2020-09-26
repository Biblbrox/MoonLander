#ifndef MOONLANDER_TIMER_HPP
#define MOONLANDER_TIMER_HPP

#include <SDL_types.h>
#include <vector>

namespace utils
{
    class Timer
    {
    public:
        Timer();

        void start();
        void stop();
        void pause();
        void unpause();

        Uint32 getTicks() const;

        bool isStarted() const;
        bool isPaused() const;

    private:
        Uint32 m_startTicks;
        Uint32 m_pausedTicks;

        bool m_paused;
        bool m_started;
    };
}


#endif //MOONLANDER_TIMER_HPP
