#ifndef MOONLANDER_SDLEXCEPTION_HPP
#define MOONLANDER_SDLEXCEPTION_HPP

#include <string>
#include "basegameexception.h"

/**
 * Exception class that can be throw when some error happened
 * with sdl functions.
 */
class SdlException : public BaseGameException
{
public:
    explicit SdlException(std::string msg,
                          std::string fileLog = "moonlander_log.log",
                          Category category = Category::INTERNAL_ERROR)
            : BaseGameException(msg, fileLog, category) {}
};

#endif //MOONLANDER_SDLEXCEPTION_HPP