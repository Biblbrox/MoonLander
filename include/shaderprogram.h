#ifndef MOONLANDER_SHADERPROGRAM_H
#define MOONLANDER_SHADERPROGRAM_H

#include <GL/gl.h>

class ShaderProgram {
public:
    ShaderProgram();

    virtual ~ShaderProgram();
    virtual bool loadProgram() = 0;
    virtual void freeProgram();
    bool bind();
    void unbind();
    GLuint getProgramID();

protected:
    GLuint programID;
};

#endif //MOONLANDER_SHADERPROGRAM_H
