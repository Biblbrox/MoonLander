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
#include "constants.hpp"
#include "timer.hpp"

namespace utils {

    struct Point {
        GLfloat x;
        GLfloat y;
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

    namespace log {
        /**
         * Write msg to the standard output and log file
         * @param userdata
         * @param category
         * @param priority
         * @param msg
         */
        void log_function(void *userdata, int category, SDL_LogPriority priority, const char *msg);
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

    class RandomUniform {
    public:
        explicit RandomUniform();

        template<typename T>
        void fill_unique(const typename std::vector<T>::iterator &begin,
                         const typename std::vector<T>::iterator &end, T left, T right) {
            std::generate(begin, end, [begin, end, left, right, this]() {
                GLuint val = generate<T>(left, right);
                if (std::find(begin, end, val) != end)
                    while (std::find(begin, end, val) != end)
                        val = generate<T>(left, right);

                return val;
            });
        }

        template<typename T>
        void fill(const typename std::vector<T>::iterator &begin,
                  const typename std::vector<T>::iterator &end, T left, T right) {
            std::generate(begin, end, [left, right, this]() {
                return generate<T>(left, right);
            });
        }

        template<typename T>
        T generate(T a, T b) {
            std::uniform_int_distribution<T> dist(a, b);
            return dist(this->generator);

        }

    private:
        std::mt19937 generator;
    };

    template<>
    inline GLfloat RandomUniform::generate<GLfloat>(GLfloat a, GLfloat b)
    {
        std::uniform_real_distribution<GLfloat> dist(a, b);
        return dist(this->generator);
    }

    template<>
    inline GLdouble RandomUniform::generate<GLdouble>(GLdouble a, GLdouble b)
    {
        std::uniform_real_distribution<GLdouble> dist(a, b);
        return dist(this->generator);
    }

    template<typename T>
    inline constexpr size_t type_id()
    {
        return typeid(T).hash_code();
    }

    /**
     * Return full path to resource fileName
     * @param fileName
     * @return
     */
    inline std::string getResourcePath(const std::string &fileName) {
        return std::string(RESOURCE_PATH + fileName);
    }

    /**
     * Return full path to shader fileName
     * @param fileName
     * @return
     */
    inline std::string getShaderPath(const std::string &fileName) {
        return std::string(SHADER_PATH + fileName);
    }

    template<typename T>
    inline T getScreenWidth() {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.w;
    }

    template<typename T>
    inline T getScreenHeight() {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.h;
    }

    inline RectPoints buildRectPoints(const Rect &rect, double rot) {
        RectPoints temp_rect;
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

    /**
     * Check whether first line (p11, p12) intersect with second (p21, p22)
     * @param p11
     * @param p12
     * @param p21
     * @param p22
     * @return
     */
    inline constexpr bool lineLine(const Point &p11, const Point &p12, const Point &p21, const Point &p22) noexcept
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
        GLfloat uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
        GLfloat uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

        // if uA and uB are between 0-1, lines are colliding
        return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
    }

    /**
     * Writes shader log to shader log file and standard output
     * @param shader
     */
    inline void printShaderLog(GLuint shader)
    {
        // TODO: write info to log file
        if (glIsShader(shader)) {
            int infoLength = 0;
            int maxLength = infoLength;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            char *log_str = new char[maxLength];

            glGetShaderInfoLog(shader, maxLength, &infoLength, log_str);
            if (infoLength > 0) {
                printf("%s\n", log_str);
            }

            delete[] log_str;
        } else {
            printf("Name %d is not a shader\n", shader);
        }
    }

    /**
     * Writes program log to shader log file and standard output
     * @param program
     */
    inline void printProgramLog(GLuint program)
    {
        // TODO: write info to log file
        if (glIsProgram(program)) {
            int infoLength = 0;
            int maxLength = infoLength;

            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            char *log_str = new char[maxLength];

            glGetProgramInfoLog(program, maxLength, &infoLength, log_str);
            if (infoLength > 0) {
                printf("%s\n", log_str);
            }

            delete[] log_str;
        } else {
            printf("Name %d is not a program\n", program);
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
        GLuint shaderID = 0;
        std::string shaderString;
        std::ifstream sourceFile(path.c_str());
        if (!sourceFile.is_open())
            throw std::runtime_error((boost::format("Can't open shader source file %1%\n") % path).str());

        if (sourceFile) {
            shaderString.assign(std::istreambuf_iterator<char>(sourceFile),
                                std::istreambuf_iterator<char>());

            shaderID = glCreateShader(shaderType);
            const GLchar *shaderSource = shaderString.c_str();
            glShaderSource(shaderID, 1, (const GLchar **) &shaderSource, NULL);
            glCompileShader(shaderID);

            GLint shaderCompiled = GL_FALSE;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
            if (shaderCompiled != GL_TRUE) {
                //TODO: write info to log file
                printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
                printShaderLog(shaderID);
                glDeleteShader(shaderID);
                throw std::runtime_error((boost::format("Unable to compile shader %1%\n\nSource:\n%2%\n")
                                          % shaderID % shaderSource).str());
            }
        } else {
            printf("Unable to open file %s\n", path.c_str());
        }

        return shaderID;
    }

    /**
     * Return Surface format
     * if surface format can't be recognized 0 will be returned
     * @param surface
     * @return
     */
    inline GLenum getSurfaceFormatInfo(const SDL_Surface &surface)
    {
        GLenum format = GL_RGBA;
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
        } else {
            return 0;
        }

        return format;
    }
}

#endif //MOONLANDER_UTILS_H
