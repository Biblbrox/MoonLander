#ifndef MOONLANDER_UTILS_H
#define MOONLANDER_UTILS_H

#include <SDL_render.h>
#include <string>
#include <iostream>
#include <SDL_image.h>
#include <random>
#include "types.hpp"
#include "freelable.hpp"

namespace Utils {

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
}

#endif //MOONLANDER_UTILS_H
