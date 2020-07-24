#ifndef MOONLANDER_UTILS_H
#define MOONLANDER_UTILS_H

#include <SDL_render.h>
#include <string>
#include <iostream>
#include <SDL_image.h>
#include <random>
#include <GL/gl.h>
#include <GL/glu.h>
#include "types.hpp"
#include "freelable.hpp"

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
        GLuint w;
        GLuint h;
    };

    class RandomUniform {
    public:
        explicit RandomUniform();
        int generate(int a, int b);
    private:
        std::mt19937 generator;
    };

    /**
     * Return full path to resource fileName
     * @param fileName
     * @return
     */
    inline std::string getResourcePath(const std::string& fileName)
    {
        return std::string(RESOURCE_PATH + fileName);
    }

    inline void cleanup(Freelable& freelable)
    {
        freelable.free();
    }

    constexpr double pi()
    {
        return std::atan(1) * 4;
    }

    constexpr GLfloat radians(GLfloat degrees)
    {
        return degrees * pi() / 180.f;
    }

    constexpr GLfloat degrees(GLfloat radians)
    {
        return radians / pi() * 180.f;
    }

    inline int getScreenWidth()
    {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.w;
    }

    inline int getScreenHeight()
    {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        return dm.h;
    }

    /**
     *
     * @param pixels
     * @param width
     * @param height
     * @return id of created texture
     */
    GLuint loadTextureFromPixels32(GLuint *pixels, GLuint width, GLuint height);
}

#endif //MOONLANDER_UTILS_H
