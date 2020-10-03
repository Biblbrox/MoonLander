#include "../../include/exceptions/sdlexception.h"

SdlException::SdlException(std::string msg) : m_message(std::move(msg))
{

}

const char* SdlException::what() const noexcept
{
    return m_message.c_str();
}