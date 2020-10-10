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
        explicit Random() : m_generator(std::time(nullptr)) {}

        /**
         * Fill sequence with unique elements
         * fix_near parameter controls whether near elements
         * can differ by 1 from each other.
         * @tparam T
         * @tparam ForwardIterator
         * @param begin
         * @param end
         * @param left
         * @param right
         * @param fix_near
         */
        template<typename T, class ForwardIterator>
        void fill_unique(ForwardIterator begin, ForwardIterator end, T left,
                         T right, bool fix_near = false)
        {
            std::generate(begin, end,
                          [begin, end, left, right, fix_near, this]() {
                              GLuint val = generateu<T>(left, right);
                              if (fix_near) {
                                  while (std::count(begin, end, val) != 0
                                         || std::count(begin, end, val - 1) != 0
                                         || std::count(begin, end, val + 1) != 0)
                                      val = generateu<T>(left, right);
                              } else {
                                  while (std::count(begin, end, val) != 0)
                                      val = generateu<T>(left, right);
                              }

                              return val;
                          });
        }

        /**
         * Fill sequence with gassian distributed numbers
         * @tparam T
         * @tparam ForwardIterator
         * @param begin
         * @param end
         * @param mean
         * @param deviation
         */
        template<typename T, class ForwardIterator>
        void fill_gauss(ForwardIterator begin, ForwardIterator end, T mean,
                        T deviation)
        {
            std::generate(begin, end, [mean, deviation, this]() {
                return generaten<T>(mean, deviation);
            });
        }

        /**
         * Fill sequence with uniform distributed numbers
         * @tparam T
         * @tparam ForwardIterator
         * @param begin
         * @param end
         * @param left
         * @param right
         */
        template<typename T, class ForwardIterator>
        void fill(ForwardIterator begin, ForwardIterator end, T left, T right)
        {
            std::generate(begin, end, [left, right, this]() {
                return generateu<T>(left, right);
            });
        }

        /**
         * Generate number of type T with uniform distribution
         * in range [a, b]
         * @tparam T
         * @param a
         * @param b
         * @return
         */
        template<typename T>
        T generateu(T a, T b)
        {
            std::uniform_int_distribution<T> dist(a, b);
            return dist(this->m_generator);
        }

        /**
         * Generate number of type T with gaussian distribution
         * @tparam T
         * @param mean - mean value
         * @param std - standard deviation
         * @return
         */
        template<typename T>
        T generaten(T mean, T std)
        {
            static_assert(std::is_floating_point_v<T>,
                          "Template parameter of generaten must be floating point");
            std::normal_distribution<T> dist(mean, std);
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
