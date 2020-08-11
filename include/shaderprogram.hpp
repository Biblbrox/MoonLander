#ifndef MOONLANDER_SHADERPROGRAM_HPP
#define MOONLANDER_SHADERPROGRAM_HPP

#include <GL/gl.h>

class ShaderProgram {
public:
    ShaderProgram();

    virtual ~ShaderProgram();
    virtual bool loadProgram() = 0;
    virtual void freeProgram();
    bool bind() const;
    void unbind();
    GLuint getProgramID();

protected:
    GLuint programID;
};

#endif //MOONLANDER_SHADERPROGRAM_HPP
