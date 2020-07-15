#ifndef MOONLANDER_TIMER_H
#define MOONLANDER_TIMER_H

#include <SDL_types.h>

namespace Utils {

    class Timer {
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
        Uint32 startTicks;
        Uint32 pausedTicks;

        bool paused;
        bool started;
    };

}


#endif //MOONLANDER_TIMER_H
