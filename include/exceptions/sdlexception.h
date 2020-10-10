#ifndef MOONLANDER_SDLEXCEPTION_H
#define MOONLANDER_SDLEXCEPTION_H

#include <exception>
#include <string>

/**
 * Exception class that can be throw when some error happened
 * with sdl functions.
 */
class SdlException : public std::exception
{
public:
    explicit SdlException(std::string msg);
    const char* what() const noexcept override;
private:
    std::string m_message;
};

#endif //MOONLANDER_SDLEXCEPTION_H
