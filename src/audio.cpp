#include <audio.h>
#include <iostream>

void utils::audio::Audio::setMusic(const std::string &musicPath) {
    if (!std::filesystem::exists(musicPath)) {
        // TODO: throw exception
    }

    Mix_Music *mus = Mix_LoadMUS(musicPath.c_str());
    if (mus == nullptr) {
        // TODO: throw exception
    }

    music = mus;
}

void utils::audio::Audio::playChunk(int channel, size_t idx, int loops, bool faded)
{
    assert(channel >= -1);
    assert(idx < chunks.size());
    if (faded) {
        Mix_FadeInChannel(channel, chunks[idx], loops, fade_in);
    } else {
        if (Mix_Paused(channel))
            Mix_Resume(channel);
        else
            Mix_PlayChannel(channel, chunks[idx], loops);
    }
}

void utils::audio::Audio::pauseChannel(int channel, bool faded)
{
    assert(channel >= -1);
    if (faded)
        Mix_FadeOutChannel(channel, fade_out);
    else
        Mix_Pause(channel);
}

void utils::audio::Audio::resumeChannel(int channel)
{
    assert(channel >= -1);

    Mix_Resume(channel);
}

void utils::audio::Audio::playMusic()
{
    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(music, -1);
    } else {
        if (Mix_PausedMusic())
            Mix_ResumeMusic();
    }
}

void utils::audio::Audio::pauseMusic()
{
    if (Mix_PlayingMusic())
        Mix_PauseMusic();
}

void utils::audio::Audio::addChunk(const std::string &chunkPath)
{
    if (!std::filesystem::exists(chunkPath)) {
        // TODO: throw exception
        std::cerr << "chunk not found" << std::endl;
    }

    Mix_Chunk *chunk = Mix_LoadWAV(chunkPath.c_str());
    if (chunk == nullptr) {
        std::cerr << "unable to load chunk" << std::endl;
        // TODO: throw exception
    }

    chunks.push_back(chunk);
}

bool utils::audio::Audio::isChannelPaused(int channel)
{
    assert(channel >= -1);
    return Mix_Paused(channel);
}

bool utils::audio::Audio::isChannelPlaying(int channel)
{
    assert(channel >= -1);
    return Mix_Playing(channel);
}

void utils::audio::Audio::setFadeIn(GLfloat in)
{
    fade_in = in;
}

void utils::audio::Audio::setFadeOut(GLfloat out)
{
    fade_out = out;
}

utils::audio::Audio::Audio(): fade_in(0.0), fade_out(0.0) {}

void utils::audio::Audio::haltChannel(int channel, bool faded)
{
    assert(channel >= -1);
    if (faded)
        Mix_FadeOutChannel(channel, fade_out);
    else
        Mix_HaltChannel(channel);
}
