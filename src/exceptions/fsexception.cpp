#include "exceptions/fsexception.hpp"
#include <string>
#include <utility>

FSException::FSException(std::string msg) : message(std::move(msg)) {}

const char* FSException::what() const noexcept
{
    return message.c_str();
}