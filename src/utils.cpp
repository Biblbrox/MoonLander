#include <ctime>
#include "../include/utils.hpp"

int Utils::RandomUniform::generate(int a, int b)
{
    std::uniform_int_distribution<int> dist(a, b);
    return dist(this->generator);
}

Utils::RandomUniform::RandomUniform() : generator(std::time(nullptr))
{

}
