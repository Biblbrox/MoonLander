#ifndef MOONLANDER_AUDIOTOOLS_H
#define MOONLANDER_AUDIOTOOLS_H

#include <SDL_mixer.h>
#include <string>
#include <filesystem>
#include <vector>
#include <cassert>

namespace utils::audio {
    class Audio {
    public:
        Audio() {};

        void addChunk(const std::string& chunkPath) {
            if (!std::filesystem::exists(chunkPath)) {
                // TODO: throw exception
            }

            Mix_Chunk *chunk = Mix_LoadWAV(chunkPath.c_str());
            if (chunk == nullptr) {
                // TODO: throw exception
            }

            chunks.push_back(chunk);
        }

        void setMusic(const std::string& musicPath)
        {
            if (!std::filesystem::exists(musicPath)) {
                // TODO: throw exception
            }

            Mix_Music *mus = Mix_LoadMUS(musicPath.c_str());
            if (mus == nullptr) {
                // TODO: throw exception
            }

            music = mus;
        }

        void playChunk(int channel, size_t idx)
        {
            assert(channel >= -1);
            assert(idx < chunks.size());

            Mix_PlayChannel(channel, chunks[idx], -1);
        }

        void stopChannel(int channel)
        {
            assert(channel >= -1);

            Mix_Pause(channel);
        }

        void resumeChannel(int channel)
        {
            assert(channel >= -1);

            Mix_Resume(channel);
        }

        void playMusic()
        {
            if (!Mix_PlayingMusic()) {
                Mix_PlayMusic(music, -1);
            } else {
                if (Mix_PausedMusic())
                    Mix_ResumeMusic();
            }
        }

        void stopMusic()
        {
            if (Mix_PlayingMusic())
                Mix_PauseMusic();
        }

    private:
        std::vector<Mix_Chunk *> chunks;
        Mix_Music* music;
    }
}

#endif //MOONLANDER_AUDIOTOOLS_H
