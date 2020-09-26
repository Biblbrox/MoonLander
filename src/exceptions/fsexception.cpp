#include "exceptions/fsexception.hpp"
#include <string>
#include <utility>

FSException::FSException(std::string msg) : m_message(std::move(msg)) {}

const char* FSException::what() const noexcept
{
    return m_message.c_str();
}