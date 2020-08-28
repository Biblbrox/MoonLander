#include <logger.hpp>
#include <fstream>
#include <iostream>

void utils::log::Logger::write(const std::string &file_name, Category category, const std::string &msg)
{
    std::ofstream file(file_name);
    std::string category_str;
    switch (category) {
        case Category::INTERNAL_ERROR:
            category_str = "Internal error: ";
            break;
        case Category::FILE_ERROR:
            category_str = "File IO error: ";
            break;
        case Category::UNEXPECTED_ERROR:
            category_str = "Unexpected error: ";
            break;
        default:
            category_str = "Internal error: ";
            break;
    }
    std::string message = category_str + msg;
    file.write(message.c_str(), message.size());
    file.close();
    std::cerr << message << "\n";
}