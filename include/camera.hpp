#ifndef MOONLANDER_CAMERA_HPP
#define MOONLANDER_CAMERA_HPP

#include <GL/gl.h>

class Camera {
public:
    Camera();
    void lookAt(GLfloat x, GLfloat y);

    GLfloat getX() const;
    GLfloat getY() const;

    void setX(GLfloat x);
    void setY(GLfloat y);

    void translate(GLfloat x, GLfloat y);

private:
    GLfloat x;
    GLfloat y;
};

#endif //MOONLANDER_CAMERA_HPP