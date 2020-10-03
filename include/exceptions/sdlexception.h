#ifndef MOONLANDER_SDLEXCEPTION_H
#define MOONLANDER_SDLEXCEPTION_H

#include <exception>
#include <string>

class SdlException : public std::exception
{
public:
    explicit SdlException(std::string msg);
    const char* what() const noexcept override;
private:
    std::string m_message;
};

#endif //MOONLANDER_SDLEXCEPTION_H
