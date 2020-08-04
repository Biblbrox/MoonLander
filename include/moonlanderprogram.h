#ifndef MOONLANDER_MOONLANDERPROGRAM_H
#define MOONLANDER_MOONLANDERPROGRAM_H

#include <glm/mat4x4.hpp>
#include "shaderprogram.h"

class MoonLanderProgram: public ShaderProgram {
public:
    MoonLanderProgram();

    void setProjection(glm::mat4 matrix);
    void setModel(glm::mat4 matrix);
    void setView(glm::mat4 matrix);

    void leftMultModelView(glm::mat4 matrix);
    void leftMultProjection(glm::mat4 matrix);

    void updateProjection();
    void updateView();
    void updateModel();

    bool loadProgram() override;

private:
    glm::mat4 projectionMatrix;
    GLint projectionLoc;

    glm::mat4 modelMatrix;
    GLint modelLoc;

    glm::mat4 viewMatrix;
    GLint viewLoc;
};

#endif //MOONLANDER_MOONLANDERPROGRAM_H
