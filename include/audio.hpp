#ifndef MOONLANDER_AUDIO_HPP
#define MOONLANDER_AUDIO_HPP

#include <SDL_mixer.h>
#include <string>
#include <filesystem>
#include <vector>
#include <cassert>
#include <GL/glew.h>
#include <functional>

using chunkDeleter = std::function<void(Mix_Chunk*)>;
using musDeleter = std::function<void(Mix_Music*)>;

namespace utils::audio
{
    class Audio
    {
    public:
        Audio();
        ~Audio();

        void addChunk(const std::string &chunkPath);

        void setMusic(const std::string &musicPath);

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
        std::vector<std::unique_ptr<Mix_Chunk, chunkDeleter>> m_chunks;
        std::unique_ptr<Mix_Music, musDeleter> m_music;
        GLfloat m_fadeIn;
        GLfloat m_fadeOut;
    };
}

#endif //MOONLANDER_AUDIO_HPP
