#ifndef MOONLANDER_RANDOM_H
#define MOONLANDER_RANDOM_H

#include <vector>
#include <GL/glew.h>
#include <random>
#include <ctime>

namespace utils
{
    class Random
    {
    public:
        explicit Random() : m_generator(std::time(nullptr))
        {}

        template<typename T>
        void
        fill_unique(const typename std::vector<T>::iterator &begin,
                    const typename std::vector<T>::iterator &end, T left,
                    T right,
                    bool fix_near = false)
        {
            std::generate(begin, end,
                          [begin, end, left, right, fix_near, this]() {
                              GLuint val = generateu<T>(left, right);
                              if (fix_near) {
                                  if (val != 0) {
                                      while (std::count(begin, end, val) != 0
                                             ||
                                             std::count(begin, end, val - 1) !=
                                             0
                                             ||
                                             std::count(begin, end, val + 1) !=
                                             0)
                                          val = generateu<T>(left, right);
                                  } else {
                                      while (std::count(begin, end, val) != 0
                                             ||
                                             std::count(begin, end, val + 1) !=
                                             0)
                                          val = generateu<T>(left, right);
                                  }
                              } else {
                                  while (std::count(begin, end, val) != 0)
                                      val = generateu<T>(left, right);
                              }

                              return val;
                          });
        }

        template<typename T>
        void fill_gauss(const typename std::vector<T>::iterator &begin,
                        const typename std::vector<T>::iterator &end, T mean,
                        T deviation)
        {
            std::generate(begin, end, [mean, deviation, this]() {
                return generaten<T>(mean, deviation);
            });
        }

        template<typename T>
        void fill(const typename std::vector<T>::iterator &begin,
                  const typename std::vector<T>::iterator &end, T left, T right)
        {
            std::generate(begin, end, [left, right, this]() {
                return generateu<T>(left, right);
            });
        }

        template<typename T>
        T generateu(T a, T b)
        {
            std::uniform_int_distribution<T> dist(a, b);
            return dist(this->m_generator);
        }

        template<typename T>
        T generaten(T mean, T svariance)
        {
            static_assert(std::is_floating_point_v<T>,
                          "Template parameter of generaten must be floating point");
            std::normal_distribution<T> dist(mean, svariance);
            return dist(this->m_generator);
        }

    private:
        std::mt19937 m_generator;
    };

    template<>
    inline GLfloat Random::generateu<GLfloat>(GLfloat a, GLfloat b)
    {
        std::uniform_real_distribution<GLfloat> dist(a, b);
        return dist(m_generator);
    }

    template<>
    inline GLdouble Random::generateu<GLdouble>(GLdouble a, GLdouble b)
    {
        std::uniform_real_distribution<GLdouble> dist(a, b);
        return dist(m_generator);
    }
}

#endif //MOONLANDER_RANDOM_H
