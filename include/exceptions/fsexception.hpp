#ifndef MOONLANDER_FSEXCEPTION_HPP
#define MOONLANDER_FSEXCEPTION_HPP

#include <exception>
#include <string>

/**
 * Exception class that can be throw when happened some error
 * with file system(can't open file, no access, etc..)
 */
class FSException: public std::exception
{
public:
    explicit FSException(std::string  msg);

    const char* what() const noexcept override;

private:
    std::string m_message;
};


#endif //MOONLANDER_FSEXCEPTION_HPP
