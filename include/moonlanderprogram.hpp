#ifndef MOONLANDER_MOONLANDERPROGRAM_HPP
#define MOONLANDER_MOONLANDERPROGRAM_HPP

#include <glm/mat4x4.hpp>
#include "abstract/shaderprogram.hpp"
#include <memory>

class MoonLanderProgram: public ShaderProgram {
protected:
    static std::shared_ptr<MoonLanderProgram> instance;
public:

    //TODO: fix public constructor and make_shared problem
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
    GLint projectionLoc;

    glm::mat4 modelMatrix;
    GLint modelLoc;

    glm::mat4 viewMatrix;
    GLint viewLoc;

    glm::vec4 color;
    GLint colorLoc;

    GLint isTextureLoc;

    GLint texLoc;
};

#endif //MOONLANDER_MOONLANDERPROGRAM_HPP
