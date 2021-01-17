#ifndef MOONLANDER_BASEGAMEEXCEPTION_HPP
#define MOONLANDER_BASEGAMEEXCEPTION_HPP

#include <exception>
#include <string>

#include "utils/logger.hpp"

using utils::log::Category;
using utils::log::program_log_file_name;

class BaseGameException : public std::exception
{
public:
    explicit BaseGameException(std::string msg,
                               std::string fileLog = program_log_file_name(),
                               Category category = Category::INTERNAL_ERROR);

    const char* what() const noexcept override;
    virtual const char* fileLog() const noexcept;
    virtual Category categoryError() const noexcept;

private:
    std::string m_message;
    std::string m_fileLog;
    Category m_category;
};

#endif //MOONLANDER_BASEGAMEEXCEPTION_HPP
