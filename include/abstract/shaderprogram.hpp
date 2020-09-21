#ifndef MOONLANDER_SHADERPROGRAM_HPP
#define MOONLANDER_SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <string>

class ShaderProgram
{
public:
    explicit ShaderProgram();

    virtual ~ShaderProgram();
    virtual bool loadProgram() = 0;
    virtual void freeProgram() final;
    virtual void setFloat(const std::string& str, GLfloat value);
    virtual void setInt(const std::string &name, GLint value);

    bool bind() const;
    void unbind();
    GLuint getProgramID();

protected:
    GLuint m_programID;
};

#endif //MOONLANDER_SHADERPROGRAM_HPP
