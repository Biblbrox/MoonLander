#ifndef MOONLANDER_UTILS_H
#define MOONLANDER_UTILS_H

#include <SDL_render.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <SDL_image.h>
#include <random>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/vec3.hpp>
#include <typeinfo>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include "types.hpp"
#include "timer.hpp"

namespace Utils {

    struct Point {
        GLfloat x;
        GLfloat y;
    };

    struct VertexData2D {
        Point position;
        Point texCoord;
    };

    struct GLColor {
        GLfloat r;
        GLfloat g;
        GLfloat b;
        GLfloat a;
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

    inline glm::mat4 rotateAround(const glm::mat4& m, const glm::vec3& v, GLfloat angle)
    {
        glm::mat4 tr1 = glm::translate(m, v);
        glm::mat4 ro = glm::rotate(tr1, angle, glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 tr2 = glm::translate(ro, -v);

        return tr2;
    }

    class RandomUniform {
    public:
        explicit RandomUniform();

        template <typename T>
        void fill_unique(const typename std::vector<T>::iterator& begin,
                         const typename std::vector<T>::iterator& end, T left, T right)
        {
            std::generate(begin, end, [begin, end, left, right, this]() {
                GLuint val = generate<T>(left, right);
                if (std::find(begin, end, val) != end)
                    while (std::find(begin, end, val) != end)
                        val = generate<T>(left, right);

                return val;
            });
        }

        template <typename T>
        void fill(const typename std::vector<T>::iterator& begin,
                         const typename std::vector<T>::iterator& end, T left, T right)
        {
            std::generate(begin, end, [left, right, this]() {
                return generate<T>(left, right);
            });
        }

        template <typename T>
        T generate(T a, T b)
        {
            std::uniform_int_distribution<T> dist(a, b);
            return dist(this->generator);
        }

    private:
        std::mt19937 generator;
    };

    template <>
    inline GLfloat RandomUniform::generate<GLfloat>(GLfloat a, GLfloat b)
    {
        std::uniform_real_distribution<GLfloat> dist(a, b);
        return dist(this->generator);
    }

    template <>
    inline GLdouble RandomUniform::generate<GLdouble>(GLdouble a, GLdouble b)
    {
        std::uniform_real_distribution<GLdouble> dist(a, b);
        return dist(this->generator);
    }

    inline GLuint getFps(const Timer& fpsTimer, GLuint countFrames)
    {
        float avgFPS = countFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 200000)
            avgFPS = 0;

        return avgFPS;
    }

    inline std::vector<Point> moveVertices(const std::vector<Point>& vertices, GLfloat shift_x, GLfloat shift_y)
    {
        std::vector<Point> res(vertices);
        for (Point& p: res) {
            p.x += shift_x;
            p.y += shift_y;
        }

        return res;
    }

    template <typename T>
    inline constexpr size_t type_id()
    {
        return typeid(T).hash_code();
    }

    /**
     * Return full path to resource fileName
     * @param fileName
     * @return
     */
    inline std::string getResourcePath(const std::string& fileName)
    {
        return std::string(RESOURCE_PATH + fileName);
    }

    /**
     * Return full path to shader fileName
     * @param fileName
     * @return
     */
    inline std::string getShaderPath(const std::string& fileName)
    {
        return std::string(SHADER_PATH + fileName);
    }

    template <typename T>
    inline void shift_vector(std::vector<T>& vec, int shift)
    {
        int idx = 0;
        while (shift-- > 0) {
            for (size_t i = 0; i < vec.size(); ++i) {
                idx = (i + 1 == vec.size()) ? ((i + 1) % vec.size()) : i;

                vec[idx] = vec[i];
            }
        }
    }

    template <typename T>
    inline T getScreenWidth()
    {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.w;
    }

    template <typename T>
    inline T getScreenHeight()
    {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.h;
    }

    inline RectPoints buildRectPoints(const Rect& rect, double rot)
    {
        RectPoints temp_rect;
        GLfloat bx,by,cx,cy,dx,dy;
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

        temp_rect.a.x = x;  temp_rect.a.y = y;
        temp_rect.b.x = bx; temp_rect.b.y = by;
        temp_rect.c.x = cx; temp_rect.c.y = cy;
        temp_rect.d.x = dx; temp_rect.d.y = dy;

        return temp_rect;
    }

    inline SDL_Surface* flipVertically(const SDL_Surface *sfc)
    {
        SDL_Surface* result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
                                                   sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
                                                   sfc->format->Bmask, sfc->format->Amask);
        // Number of pixels per row
        const auto pitch = sfc->pitch;
        // Total number of pixels
        const auto pxlength = pitch*(sfc->h - 1);
        // Right end pixels
        auto pixels = static_cast<unsigned char*>(sfc->pixels) + pxlength;
        // Left pixels
        auto rpixels = static_cast<unsigned char*>(result->pixels) ;
        for(auto line = 0; line < sfc->h; ++line) {
            memcpy(rpixels,pixels,pitch);
            pixels -= pitch;
            rpixels += pitch;
        }
        return result;
    }

    /**
     *
     * @param pixels
     * @param width
     * @param height
     * @return id of created texture
     */
    GLuint loadTextureFromPixels32(GLuint *pixels, GLuint width, GLuint height, GLenum textureType = GL_RGBA);

    inline unsigned int power_two_floor(unsigned int val)
    {
        unsigned int power = 2, nextVal = power*2;

        while((nextVal *= 2) <= val)
            power*=2;

        return power*2;
    }

    inline constexpr bool lineLine(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
    {
        GLfloat x1 = a1.x;
        GLfloat x2 = a2.x;
        GLfloat x3 = b1.x;
        GLfloat x4 = b2.x;

        GLfloat y1 = a1.y;
        GLfloat y2 = a2.y;
        GLfloat y3 = b1.y;
        GLfloat y4 = b2.y;

        // calculate the distance to intersection point
        GLfloat uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        GLfloat uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

        // if uA and uB are between 0-1, lines are colliding
        return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
    }

    inline void printShaderLog(GLuint shader)
    {
        if (glIsShader(shader)) {
            int infoLength = 0;
            int maxLength = infoLength;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            char* log_str = new char[maxLength];

            glGetShaderInfoLog(shader, maxLength, &infoLength, log_str);
            if (infoLength > 0) {
                printf("%s\n", log_str);
            }

            delete[] log_str;
        } else {
            printf("Name %d is not a shader\n", shader);
        }
    }

    inline void printProgramLog(GLuint program)
    {
        if (glIsProgram(program)) {
            int infoLength = 0;
            int maxLength = infoLength;

            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            char* log_str = new char[maxLength];

            glGetProgramInfoLog(program, maxLength, &infoLength, log_str);
            if (infoLength > 0) {
                printf("%s\n", log_str);
            }

            delete[] log_str;
        } else {
            printf("Name %d is not a program\n", program);
        }
    }

    inline GLuint loadShaderFromFile(const std::string& path, GLenum shaderType)
    {
        GLuint shaderID = 0;
        std::string shaderString;
        std::ifstream sourceFile(path.c_str());

        if (sourceFile) {
            shaderString.assign(std::istreambuf_iterator<char>(sourceFile),
                                std::istreambuf_iterator<char>());

            shaderID = glCreateShader(shaderType);
            const GLchar* shaderSource = shaderString.c_str();
            glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);
            glCompileShader(shaderID);

            GLint shaderCompiled = GL_FALSE;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
            if (shaderCompiled != GL_TRUE) {
                printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource );
                printShaderLog( shaderID );
                glDeleteShader( shaderID );
                shaderID = 0;
            }
        } else {
            printf( "Unable to open file %s\n", path.c_str() );
        }

        return shaderID;
    }

    /**
     * Return Surface format
     * @param surface
     * @return
     */
    inline GLenum getSurfaceFormatInfo(const SDL_Surface& surface)
    {
        GLenum format = GL_RGBA;
        GLint  nOfColors;
        nOfColors = surface.format->BytesPerPixel;
        if(nOfColors == 4)     // contains an alpha channel
        {
            if(surface.format->Rmask == 0x000000ff)
                format = GL_RGBA;
            else
                format = GL_BGRA;
        }
        else if(nOfColors == 3)     // no alpha channel
        {
            if(surface.format->Rmask == 0x000000ff)
                format = GL_RGB;
            else
                format = GL_BGR;
        }
        else
        {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }

        return format;
    }
}

#endif //MOONLANDER_UTILS_H
