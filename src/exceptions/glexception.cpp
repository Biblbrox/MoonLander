#include "exceptions/glexception.hpp"

GLException::GLException(std::string msg) : message(std::move(msg)) {}

const char* GLException::what() const noexcept
{
    return message.c_str();
}
