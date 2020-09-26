#ifndef MOONLANDER_GLEXCEPTION_HPP
#define MOONLANDER_GLEXCEPTION_HPP

#include <exception>
#include <string>

class GLException: public std::exception
{
public:
    explicit GLException(std::string msg);
    const char* what() const noexcept override;

private:
    std::string m_message;
};


#endif //MOONLANDER_GLEXCEPTION_HPP
