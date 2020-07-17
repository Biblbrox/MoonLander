#ifndef MOONLANDER_2DVECTOR_H
#define MOONLANDER_2DVECTOR_H

template <typename T>
struct Vector2D {
    Vector2D(T x, T y) : x(x), y(y) {};
    T x;
    T y;
};

#endif //MOONLANDER_2DVECTOR_H
