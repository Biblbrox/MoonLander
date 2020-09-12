#ifndef MOONLANDER_LOGGER_HPP
#define MOONLANDER_LOGGER_HPP

#include <string>

namespace utils::log
{

    enum class Category
    {
        INTERNAL_ERROR,
        FILE_ERROR,
        UNEXPECTED_ERROR,
        INFO,
        INITIALIZATION_ERROR
    };

    class Logger
    {
    public:
        static void write(const std::string &file_name, Category category, const std::string &msg);
    };
}

#endif //MOONLANDER_LOGGER_HPP
