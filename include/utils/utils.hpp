#ifndef MOONLANDER_UTILS_H
#define MOONLANDER_UTILS_H

#include <SDL_render.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <SDL_image.h>
#include <random>
#include <glm/vec3.hpp>
#include <typeinfo>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <boost/format.hpp>
#include <components/positioncomponent.hpp>
#include <exceptions/fsexception.hpp>
#include <exceptions/glexception.hpp>
#include <sstream>
#include "constants.hpp"
#include "timer.hpp"
#include "logger.hpp"

using glm::vec2;
using boost::format;
using utils::log::Logger;
using utils::log::Category;

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

    constexpr const char* shader_log_file_name()
    {
        return "shader_log.log";
    }

    constexpr const char* program_log_file_name()
    {
        return "moonlander_log.log";
    }

    inline std::vector<std::string> split_to_lines(const std::string& str)
    {
        std::vector<std::string> res;
        std::istringstream str_stream(str);
        std::string line;
        while (std::getline(str_stream, line))
            res.push_back(line);

        return res;
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

    /**
     * TypeList declaration
     * @tparam Args
     */
    template <typename... Args>
    struct TypeList;

    template <typename H, typename... T>
    struct TypeList<H, T...>
    {
        using Head = H;
        using Tail = TypeList<T...>;
    };

    template <>
    struct TypeList<> {};

    template <typename TypeList>
    struct Length
    {
        static int const value = 1 + Length<typename TypeList::Tail>::value;
    };

    template <>
    struct Length <TypeList<>>
    {
        static int const value = 0;
    };

    template <class TL, class UnFunctor, class BinFunctor>
    auto typeListReduce(UnFunctor&& unfunc, BinFunctor&& binfunc)
    {
        static_assert(Length<TL>::value >= 2,
                "Length<TypeList<Args...>>::value >= 2");

        typename TL::Head val;
        auto res = unfunc(val);

        if constexpr (Length<TL>::value == 2) {
            typename TL::Tail::Head tmp;
            return binfunc(res, unfunc(tmp));
        } else {
            return binfunc(res, typeListReduce<typename TL::Tail>(unfunc, binfunc));
        }
    }

    template<typename T>
    constexpr size_t type_id() noexcept
    {
        return typeid(T).hash_code();
    }

    /**
     * Namespace for logging functions
     */
    namespace log
    {
        /**
         * Writes shader log to shader log file and standard output
         * @param shader
         */
        inline void printShaderLog(GLuint shader)
        {
            if (glIsShader(shader)) {
                int infoLength = 0;
                int maxLength = infoLength;

                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                char *log_str = new char[maxLength];

                glGetShaderInfoLog(shader, maxLength, &infoLength, log_str);
                if (infoLength > 0) {
                    log::Logger::write(shader_log_file_name(),
                                       log::Category::INTERNAL_ERROR,
                                       (boost::format("Shader log:\n\n%s")
                                        % log_str).str());
                }

                delete[] log_str;
            } else {
                std::cerr << (boost::format("Name %d is not a shader\n")
                              % shader).str() << std::endl;
            }
        }

        /**
         * Writes program log to shader log file and standard output
         * @param program
         */
        inline void printProgramLog(GLuint program)
        {
            if (glIsProgram(program)) {
                int infoLength = 0;
                int maxLength = infoLength;

                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
                char *log_str = new char[maxLength];

                glGetProgramInfoLog(program, maxLength, &infoLength, log_str);
                if (infoLength > 0) {
                    Logger::write(shader_log_file_name(),
                                  Category::INTERNAL_ERROR,
                                  (format("Shader program log:\n\n%s")
                                   % log_str).str());
                }

                delete[] log_str;
            } else {
                Logger::write(shader_log_file_name(),Category::INTERNAL_ERROR,
                              (format("Name %1% is not a program\n")
                               % program).str());
            }
        }
    }

    /**
     *
     * @param m
     * @param v
     * @param angle
     * @return
     */
    inline glm::mat4
    rotate_around(const glm::mat4 &m, const glm::vec3 &v, GLfloat angle)
    {
        glm::mat4 tr1 = glm::translate(m, v);
        glm::mat4 ro = glm::rotate(tr1, angle, glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 tr2 = glm::translate(ro, -v);

        return tr2;
    }

    namespace physics
    {
        GLfloat altitude(const std::vector<vec2> &points, GLfloat shipX,
                              GLfloat shipY);
        GLfloat alt_from_surface(const std::vector<vec2> &line_points,
                                 GLfloat x, GLfloat alt);
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

    constexpr unsigned int power_two(unsigned int val) noexcept
    {
        unsigned int power = 2, nextVal = power * 2;

        while ((nextVal *= 2) <= val)
            power *= 2;

        return power * 2;
    }

     /**
     * Load shader from file by specific path
     * shaderType param may of the supported shader types
     * If shader can't be loaded (file not found or bad read access)
     * or can't be compiled std::runtime_error exception will be thrown.
     * @param path
     * @param shaderType
     * @return
     */
     inline GLuint loadShaderFromFile(const std::string &path, GLenum shaderType)
     {
         assert(!path.empty() && "Empty file path");
         GLuint shaderID = 0;
         std::string shaderString;
         std::ifstream sourceFile(path.c_str());
         if (!sourceFile.is_open())
             throw FSException(
                     (format("Can't open shader source file %1%\n")
                      % path).str());

         shaderString.assign(std::istreambuf_iterator<char>(sourceFile),
                             std::istreambuf_iterator<char>());

         shaderID = glCreateShader(shaderType);
         const GLchar *shaderSource = shaderString.c_str();
         glShaderSource(shaderID, 1, (const GLchar**) &shaderSource, NULL);
         glCompileShader(shaderID);

         GLint shaderCompiled = GL_FALSE;
         glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
         if (shaderCompiled != GL_TRUE) {
             glDeleteShader(shaderID);
             sourceFile.close();
             throw GLException(
                     (format("Unable to compile shader %1%\n\nSource:\n%2%\n")
                      % shaderID % shaderSource).str());
         }

         sourceFile.close();
         return shaderID;
    }

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
