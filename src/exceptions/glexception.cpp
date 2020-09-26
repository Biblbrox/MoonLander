#include "exceptions/glexception.hpp"

GLException::GLException(std::string msg) : m_message(std::move(msg)) {}

const char* GLException::what() const noexcept
{
    return m_message.c_str();
}
