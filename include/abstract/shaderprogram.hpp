#ifndef MOONLANDER_SHADERPROGRAM_HPP
#define MOONLANDER_SHADERPROGRAM_HPP

#include <GL/glew.h>

class ShaderProgram {
public:
    explicit ShaderProgram();

    virtual ~ShaderProgram();
    virtual bool loadProgram() = 0;
    virtual void freeProgram() final;
    bool bind() const;
    void unbind();
    GLuint getProgramID();

protected:
    GLuint programID;
};

#endif //MOONLANDER_SHADERPROGRAM_HPP
