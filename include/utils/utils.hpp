#ifndef MOONLANDER_UTILS_H
#define MOONLANDER_UTILS_H

#include <GL/glew.h>
#include <string>
#include <glm/vec2.hpp>
#include <typeinfo>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <camera.hpp>
#include <SDL.h>
#include <SDL_video.h>
#include "constants.hpp"

using glm::vec2;

namespace utils
{
    struct Position
    {
        GLfloat x;
        GLfloat y;
        GLfloat angle;
    };

    struct Rect
    {
        GLfloat x;
        GLfloat y;
        GLfloat w;
        GLfloat h;
    };

    struct RectPoints
    {
        vec2 a;
        vec2 b;
        vec2 c;
        vec2 d;
    };

    inline std::vector<std::string> split_to_lines(const std::string& str)
    {
        std::vector<std::string> res;
        boost::split(res, str, boost::is_any_of("\n"), boost::token_compress_on);
        return res;
    }

    inline glm::vec2 fix_coords(const vec2& coords, const vec2& camPos)
    {
        return {coords.x + camPos.x, coords.y + camPos.y};
    }

    /**
     * Divide clip by multiple small clips.
     * Number of clips = num_x * num_y.
     * @param clip
     * @param num_x
     * @param num_y
     * @return
     */
    std::vector<Rect>
    inline generate_clips(Rect clip, size_t num_x, size_t num_y)
    {
        GLfloat part_width = clip.w / num_x;
        GLfloat part_height = clip.h / num_y;

        std::vector<Rect> clips;
        clips.reserve(num_x * num_y);
        for (GLfloat y = clip.y; y < clip.y + clip.h; y += part_height)
            for (GLfloat x = clip.x; x < clip.x + clip.w; x += part_width)
                clips.push_back({x, y, part_width, part_height});

        return clips;
    }

    template<typename T>
    constexpr size_t type_id() noexcept
    {
        return typeid(T).hash_code();
    }

    namespace physics
    {
        GLfloat altitude(const std::vector<vec2>& points, GLfloat x, GLfloat y);
        GLfloat coord_of_alt(const std::vector<vec2>& points, GLfloat x,
                             GLfloat height);
    }

    /**
     * Return full path to resource fileName
     * @param fileName
     * @return
     */
    inline std::string getResourcePath(const std::string &fileName)
    {
        return std::string(RESOURCE_PATH + fileName);
    }

    /**
     * Return full path to shader fileName
     * @param fileName
     * @return
     */
    inline std::string getShaderPath(const std::string &fileName)
    {
        return std::string(SHADER_PATH + fileName);
    }

    /**
     * Both functions getScreenWidth and getScreenHeight can
     * be called after SDL2 initialization. Otherwise 0 will be returned
     * @tparam T
     * @return
     */
    template<typename T>
    inline T getScreenWidth() noexcept
    {
        if (SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO)
            return 0;

        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.w;
    }

    template<typename T>
    inline T getScreenHeight() noexcept
    {
        if (SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO)
            return 0;

        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.h;
    }

    /**
     * Flip verticaly SDL_Surface
     * @param sfc
     * @return
     */
    inline SDL_Surface* flipVertically(const SDL_Surface* const sfc)
    {
        assert(sfc != nullptr);
        SDL_Surface *result =
                SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
                                     sfc->format->BytesPerPixel * 8,
                                     sfc->format->Rmask,
                                     sfc->format->Gmask,
                                     sfc->format->Bmask, sfc->format->Amask);
        // Number of pixels per row
        const auto pitch = sfc->pitch;
        // Total number of pixels
        const auto pxlength = pitch * (sfc->h - 1);
        // Right end pixels
        auto pixels = static_cast<unsigned char *>(sfc->pixels) + pxlength;
        // Left pixels
        auto rpixels = static_cast<unsigned char *>(result->pixels);
        for (auto line = 0; line < sfc->h; ++line) {
            memcpy(rpixels, pixels, pitch);
            pixels -= pitch;
            rpixels += pitch;
        }

        return result;
    }

    /**
     * Load opengl texture from pixels with specific format.
     * Result texture has RGBA format.
     * If function can't load texture std::runtime_error exception will be thrown.
     * @param pixels
     * @param width
     * @param height
     * @param format
     * @return
     * */
    GLuint loadTextureFromPixels32(const GLuint *pixels, GLuint width, GLuint height,
                                   GLenum textureType = GL_RGBA);


     /**
     * Load shader from file by specific path
     * shaderType param may of the supported shader types
     * If shader can't be loaded (file not found or bad read access)
     * or can't be compiled std::runtime_error exception will be thrown.
     * @param path
     * @param shaderType
     * @return
     */
     GLuint loadShaderFromFile(const std::string &path, GLenum shaderType);

    /**
     * Return Surface format
     * if surface format can't be recognized 0 will be returned
     * @param surface
     * @return
     */
    inline GLenum getSurfaceFormatInfo(const SDL_Surface &surface) noexcept
    {
        GLenum format = 0;
        GLint nOfColors;
        nOfColors = surface.format->BytesPerPixel;
        if (nOfColors == 4) {     // contains an alpha channel
            if (surface.format->Rmask == 0x000000ff)
                format = GL_RGBA;
            else
                format = GL_BGRA;
        } else if (nOfColors == 3) {     // no alpha channel
            if (surface.format->Rmask == 0x000000ff)
                format = GL_RGB;
            else
                format = GL_BGR;
        }

        return format;
    }
}

#endif //MOONLANDER_UTILS_H
