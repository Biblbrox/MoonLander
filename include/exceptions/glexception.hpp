#ifndef MOONLANDER_GLEXCEPTION_HPP
#define MOONLANDER_GLEXCEPTION_HPP

#include <exception>
#include <string>

/**
 * Exception class that can be throw when happened some error
 * with gl functions.
 */
class GLException: public std::exception
{
public:
    explicit GLException(std::string msg);
    const char* what() const noexcept override;

private:
    std::string m_message;
};


#endif //MOONLANDER_GLEXCEPTION_HPP
