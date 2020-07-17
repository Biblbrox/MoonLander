#ifndef MOONLANDER_CUTILS_H
#define MOONLANDER_CUTILS_H

#include <glm/glm.hpp>

class CUtils {
public:
    CUtils() = delete;

    static void initOnceSDL2();
    static void validateSDL2Errors();
    static void initGL(const int width, const int height);
};

#endif //MOONLANDER_CUTILS_H
