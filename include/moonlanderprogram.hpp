#ifndef MOONLANDER_MOONLANDERPROGRAM_HPP
#define MOONLANDER_MOONLANDERPROGRAM_HPP

#include <glm/mat4x4.hpp>
#include "abstract/shaderprogram.hpp"
#include <memory>
#include <functional>

class MoonLanderProgram: public ShaderProgram
{
protected:
    static std::shared_ptr<MoonLanderProgram> instance;
public:

    //TODO: fix public constructor and make_shared problem
    //TODO: fix ugly design
    MoonLanderProgram();

    static std::shared_ptr<MoonLanderProgram> getInstance()
    {
        if (!instance) {
            instance = std::make_shared<MoonLanderProgram>();
        }

        return instance;
    }

    ~MoonLanderProgram();

    void setProjection(glm::mat4 matrix);
    void setModel(glm::mat4 matrix);
    void setView(glm::mat4 matrix);
    void setColor(glm::vec4 color);
    void setTextureRendering(bool isTexture);
    void setTexture(int texture);

    void switchToLines();
    void switchToPoints();
    void switchToTriangles();

    void rebindUniforms();

    glm::mat4 getView() const;
    glm::mat4 getModel() const;
    glm::mat4 getProjection() const;

    void leftMultView(glm::mat4 matrix);
    void leftMultModel(glm::mat4 matrix);
    void leftMultProjection(glm::mat4 matrix);

    void updateProjection();
    void updateView();
    void updateModel();
    void updateColor();

    bool loadProgram() override;

private:
    glm::mat4 projectionMatrix;

    glm::mat4 modelMatrix;

    glm::mat4 viewMatrix;

    glm::vec4 color;

    GLint texLoc;
    GLint colorLoc;

    GLuint programs[3];
    GLuint cur_program;

    // First for TextureData, second for Matrices.
    GLuint uniformPoints[2];
    GLuint uniformLines[2];
    GLuint uniformTriangles[2];

    GLuint matricesUBO;
    GLuint textureDataUBO;

    int isTextureRender;

    void remove_programs();
};

#endif //MOONLANDER_MOONLANDERPROGRAM_HPP
