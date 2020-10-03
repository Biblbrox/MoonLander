#include <audio.hpp>
#include <boost/format.hpp>
#include <exceptions/fsexception.hpp>
#include <exceptions/sdlexception.h>

using boost::format;

void utils::audio::Audio::setMusic(const std::string &musicPath)
{
    if (!std::filesystem::exists(musicPath))
        throw FSException((format("File %s doesn't exists\n") % musicPath).str());

    Mix_Music *mus = Mix_LoadMUS(musicPath.c_str());
    if (mus == nullptr)
        throw SdlException((format("Unable to load music %s\n") % musicPath).str());

    m_music = mus;
}

void
utils::audio::Audio::playChunk(int channel, size_t idx, int loops, bool faded)
{
    assert(channel >= -1);
    assert(idx < m_chunks.size());
    if (faded) {
        Mix_FadeInChannel(channel, m_chunks[idx], loops, m_fadeIn);
    } else {
        if (Mix_Paused(channel))
            Mix_Resume(channel);
        else
            Mix_PlayChannel(channel, m_chunks[idx], loops);
    }
}

void utils::audio::Audio::pauseChannel(int channel, bool faded)
{
    assert(channel >= -1);
    if (faded)
        Mix_FadeOutChannel(channel, m_fadeOut);
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
        Mix_PlayMusic(m_music, -1);
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
    if (!std::filesystem::exists(chunkPath))
        throw FSException((format("File %s doesn't exists\n") % chunkPath).str());

    Mix_Chunk *chunk = Mix_LoadWAV(chunkPath.c_str());
    if (chunk == nullptr)
        throw SdlException((format("Unable to load chunk %s\n") % chunkPath).str());

    m_chunks.push_back(chunk);
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
    m_fadeIn = in;
}

void utils::audio::Audio::setFadeOut(GLfloat out)
{
    m_fadeOut = out;
}

utils::audio::Audio::Audio(): m_fadeIn(0.0), m_fadeOut(0.0) {}

void utils::audio::Audio::haltChannel(int channel, bool faded)
{
    assert(channel >= -1);
    if (faded)
        Mix_FadeOutChannel(channel, m_fadeOut);
    else
        Mix_HaltChannel(channel);
}

