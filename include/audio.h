#ifndef MOONLANDER_AUDIO_H
#define MOONLANDER_AUDIO_H

#include <SDL_mixer.h>
#include <string>
#include <filesystem>
#include <vector>
#include <cassert>
#include <GL/glew.h>

namespace utils::audio {
    class Audio {
    public:
        Audio();

        void addChunk(const std::string& chunkPath);

        void setMusic(const std::string& musicPath);

        void playChunk(int channel, size_t idx, int loops, bool faded = false);

        void pauseChannel(int channel, bool faded = false);
        void haltChannel(int channel, bool faded = false);

        bool isChannelPaused(int channel);

        void resumeChannel(int channel);

        bool isChannelPlaying(int channel);

        void playMusic();

        void pauseMusic();
        void setFaded(bool fade);
        void setFadeIn(GLfloat in);
        void setFadeOut(GLfloat out);

    private:
        std::vector<Mix_Chunk *> chunks;
        Mix_Music* music;
        GLfloat fade_in;
        GLfloat fade_out;
    };
}

#endif //MOONLANDER_AUDIO_H
