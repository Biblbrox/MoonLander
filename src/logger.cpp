#include <logger.hpp>
#include <fstream>

void utils::log::Logger::write(const std::string &file_name, Category category, const std::string &msg)
{
    std::ofstream file(file_name);
    file.write(msg.c_str(), msg.size());
    file.close();
}