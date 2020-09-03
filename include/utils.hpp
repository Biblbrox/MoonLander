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
#include "constants.hpp"
#include "timer.hpp"
#include "logger.hpp"

namespace utils {

    constexpr const char* shader_log_file_name()
    {
        return "shader_log.log";
    }

    constexpr const char* program_log_file_name()
    {
        return "moonlander_log.log";
    }

    struct Point {
        GLfloat x;
        GLfloat y;
    };

    struct Position {
        GLfloat x;
        GLfloat y;
        GLfloat angle;
    };

    struct Rect {
        GLfloat x;
        GLfloat y;
        GLfloat w;
        GLfloat h;
    };

    struct RectPoints {
        Point a;
        Point b;
        Point c;
        Point d;
    };

    /**
     * TypeList declaration
     * @tparam Args
     */
    template <typename... Args>
    struct TypeList;

    template <typename H, typename... T>
    struct TypeList<H, T...> {
        using Head = H;
        using Tail = TypeList<T...>;
    };

    template <>
    struct TypeList<> {};

    template <typename TypeList>
    struct Length {
        static int const value = 1 + Length<typename TypeList::Tail>::value;
    };

    template <>
    struct Length <TypeList<>> {
        static int const value = 0;
    };

    template <class TL, class UnFunctor, class BinFunctor>
    auto typeListReduce(UnFunctor&& unfunc, BinFunctor&& binfunc)
    {
        static_assert(Length<TL>::value >= 2, "Length<TypeList<Args...>>::value >= 2");

        typename TL::Head val;
        auto res = unfunc(val);

        if constexpr (Length<TL>::value == 2) {
            typename TL::Tail::Head tmp;
            return binfunc(res, unfunc(tmp));
        } else {
            return binfunc(res, typeListReduce<typename TL::Tail>(unfunc, binfunc));
        }
    }

    /**
     * Namespace for logging functions
     */
    namespace log {

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
                                       (boost::format("Shader log:\n\n%s") % log_str).str());
                }

                delete[] log_str;
            } else {
                std::cerr << (boost::format("Name %d is not a shader\n") % shader).str() << std::endl;
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
                    log::Logger::write(shader_log_file_name(),
                                       log::Category::INTERNAL_ERROR,
                                       (boost::format("Shader program log:\n\n%s") % log_str).str());
                }

                delete[] log_str;
            } else {
                log::Logger::write(shader_log_file_name(),
                                   log::Category::INTERNAL_ERROR,
                                   (boost::format("Name %1% is not a program\n") % program).str());
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
    inline glm::mat4 rotate_around(const glm::mat4 &m, const glm::vec3 &v, GLfloat angle)
    {
        glm::mat4 tr1 = glm::translate(m, v);
        glm::mat4 ro = glm::rotate(tr1, angle, glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 tr2 = glm::translate(ro, -v);

        return tr2;
    }

    class RandomUniform
    {
    public:
        explicit RandomUniform();

        template<typename T>
        void fill_unique(const typename std::vector<T>::iterator &begin,
                         const typename std::vector<T>::iterator &end, T left, T right)
        {
            std::generate(begin, end, [begin, end, left, right, this]() {
                GLuint val = generateu<T>(left, right);
                if (std::count(begin, end, val) != 0)
                    while (std::find(begin, end, val) != end)
                        val = generateu<T>(left, right);

                return val;
            });
        }

        template<typename T>
        void fill_gauss(const typename std::vector<T>::iterator &begin,
                        const typename std::vector<T>::iterator &end, T mean,
                        T deviation)
        {
            std::generate(begin, end, [mean, deviation, this]() {
                return generaten<T>(mean, deviation);
            });
        }

        template<typename T>
        void fill(const typename std::vector<T>::iterator &begin,
                  const typename std::vector<T>::iterator &end, T left, T right)
        {
            std::generate(begin, end, [left, right, this]() {
                return generateu<T>(left, right);
            });
        }

        template<typename T>
        T generateu(T a, T b)
        {
            std::uniform_int_distribution<T> dist(a, b);
            return dist(this->generator);
        }

        template<typename T>
        T generaten(T mean, T svariance)
        {
            static_assert(std::is_floating_point_v<T>,
                    "Template parameter of generaten must be floating point");
            std::normal_distribution<T> dist(mean, svariance);
            return dist(this->generator);
        }

    private:
        std::mt19937 generator;
    };

    template<>
    inline GLfloat RandomUniform::generateu<GLfloat>(GLfloat a, GLfloat b)
    {
        std::uniform_real_distribution<GLfloat> dist(a, b);
        return dist(this->generator);
    }

    template<>
    inline GLdouble RandomUniform::generateu<GLdouble>(GLdouble a, GLdouble b)
    {
        std::uniform_real_distribution<GLdouble> dist(a, b);
        return dist(this->generator);
    }

    template<typename T>
    inline constexpr size_t type_id() noexcept
    {
        return typeid(T).hash_code();
    }

    namespace physics {
        GLfloat ship_altitude(const std::vector<Point> &points, GLfloat shipX, GLfloat shipY);
        GLfloat alt_from_surface(const std::vector<Point> &line_points, GLfloat x, GLfloat alt);
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
    inline SDL_Surface* flipVertically(const SDL_Surface *sfc)
    {
        assert(sfc != nullptr);
        SDL_Surface *result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
                                                   sfc->format->BytesPerPixel * 8, sfc->format->Rmask,
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
    GLuint loadTextureFromPixels32(GLuint *pixels, GLuint width, GLuint height, GLenum textureType = GL_RGBA);

    inline unsigned int power_two_floor(unsigned int val) noexcept
    {
        unsigned int power = 2, nextVal = power * 2;

        while ((nextVal *= 2) <= val)
            power *= 2;

        return power * 2;
    }

    namespace collision {
        /**
         * Check whether first line (p11, p12) intersect with second (p21, p22)
         * @param p11
         * @param p12
         * @param p21
         * @param p22
         * @return
         */
        inline constexpr bool
        lineLine(const Point &p11, const Point &p12, const Point &p21, const Point &p22) noexcept
        {
            GLfloat x1 = p11.x;
            GLfloat x2 = p12.x;
            GLfloat x3 = p21.x;
            GLfloat x4 = p22.x;

            GLfloat y1 = p11.y;
            GLfloat y2 = p12.y;
            GLfloat y3 = p21.y;
            GLfloat y4 = p22.y;

            // calculate the distance to intersection point
            GLfloat uA =
                    ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
            GLfloat uB =
                    ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

            // if uA and uB are between 0-1, lines are colliding
            return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
        }

        inline RectPoints buildRectPoints(const Rect &rect, double rot) noexcept
        {
            RectPoints temp_rect{};
            GLfloat bx, by, cx, cy, dx, dy;
            GLfloat x = rect.x;
            GLfloat y = rect.y;
            GLfloat wid = rect.w;
            GLfloat hgt = rect.h;

            bx = x + wid * cos(rot);
            by = y + wid * sin(rot);

            cx = x + wid * cos(rot) - hgt * sin(rot);
            cy = y + hgt * cos(rot) + wid * sin(rot);

            dx = x - hgt * sin(rot);
            dy = y + hgt * cos(rot);

            temp_rect.a.x = x;
            temp_rect.a.y = y;
            temp_rect.b.x = bx;
            temp_rect.b.y = by;
            temp_rect.c.x = cx;
            temp_rect.c.y = cy;
            temp_rect.d.x = dx;
            temp_rect.d.y = dy;

            return temp_rect;
        }
    }

     /**
     * Load shader from file by specific path
     * shaderType param may of the supported shader types
     * If shader can't be loaded (file not found or bad read access) or can't be compiled
     * std::runtime_error exception will be thrown. In second case information about error will be
     * writen to shader log file
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
         if (!sourceFile.is_open()) {
             log::Logger::write(shader_log_file_name(),
                                log::Category::FILE_ERROR,
                                (boost::format("Unable to open file %s\n")
                                 % path.c_str()).str());
             throw std::runtime_error((boost::format("Can't open shader source file %1%\n") % path).str());
         }

         shaderString.assign(std::istreambuf_iterator<char>(sourceFile),
                             std::istreambuf_iterator<char>());

         shaderID = glCreateShader(shaderType);
         const GLchar *shaderSource = shaderString.c_str();
         glShaderSource(shaderID, 1, (const GLchar**) &shaderSource, NULL);
         glCompileShader(shaderID);

         GLint shaderCompiled = GL_FALSE;
         glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
         if (shaderCompiled != GL_TRUE) {
             log::Logger::write(shader_log_file_name(),
                                log::Category::INTERNAL_ERROR,
                                (boost::format("Unable to compile shader %d!\n\nSource:\n%s\n")
                                 % shaderID % shaderSource).str());
             log::printShaderLog(shaderID);
             glDeleteShader(shaderID);
             throw std::runtime_error((boost::format("Unable to compile shader %1%\n\nSource:\n%2%\n")
                                       % shaderID % shaderSource).str());
         }

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
