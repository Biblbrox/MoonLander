#ifndef MOONLANDER_FPS_HPP
#define MOONLANDER_FPS_HPP

#include <cstddef>
#include <cstring>
#include <SDL_timer.h>

#define FRAMETIMES 10

class Fps
{
public:
    Fps()
    {
        std::memset(frame_times, 0, sizeof(frame_times));
        last_ticks = SDL_GetTicks();
        frame_count = 0;
        fps = 0;
    }

    void update()
    {
        size_t ticks = SDL_GetTicks();
        size_t frame_time = ticks - last_ticks;

        frame_times[frame_count % FRAMETIMES] = frame_time;
        frame_count++;

        last_ticks = ticks;

        size_t count;
        if (frame_count < FRAMETIMES)
            count = frame_count;
        else
            count = FRAMETIMES;

        fps = 0;
        for (size_t i = 0; i < count; ++i)
            fps += frame_times[i];

        fps /= count;
        fps = 1000.f / fps;
    }

    size_t get_fps() const
    {
        return fps;
    }

private:
    size_t frame_times[FRAMETIMES];
    size_t last_ticks;
    size_t frame_count;
    size_t fps;
};

#endif //MOONLANDER_FPS_HPP
