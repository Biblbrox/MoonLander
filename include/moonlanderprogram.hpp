#ifndef MOONLANDER_MOONLANDERPROGRAM_HPP
#define MOONLANDER_MOONLANDERPROGRAM_HPP

#include <glm/mat4x4.hpp>
#include "abstract/shaderprogram.hpp"
#include <memory>

class MoonLanderProgram: public ShaderProgram
{
protected:
    static std::shared_ptr<MoonLanderProgram> instance;
public:
    MoonLanderProgram();

    //TODO: fix public constructor and make_shared problem
    //TODO: fix ugly design
    static std::shared_ptr<MoonLanderProgram> getInstance()
    {
        if (!instance)
            instance = std::make_shared<MoonLanderProgram>();

        return instance;
    }

    ~MoonLanderProgram();

    void setProjection(glm::mat4 matrix);
    void setModel(glm::mat4 matrix);
    void setView(glm::mat4 matrix);
    void setColor(glm::vec4 color);
    void setTextureRendering(bool isTexture);
    void setTexture(int texture);

    /**
     * Choose geometry shader render mode
     */
    void switchToLinesAdj();
    void switchToLines();
    void switchToPoints();
    void switchToTriangles();

    /**
     * Update uniform variables locations
     */
    void rebindUniforms();

    glm::mat4 getView() const;
    glm::mat4 getModel() const;
    glm::mat4 getProjection() const;

    void leftMultView(glm::mat4 matrix);
    void leftMultModel(glm::mat4 matrix);
    void leftMultProjection(glm::mat4 matrix);

    /**
     * Write specific uniform to current program
     */
    void updateProjection();
    void updateView();
    void updateModel();
    void updateColor();

    /**
     * Init programs
     */
    void loadProgram() override;

private:
    glm::mat4 m_projectionMatrix;

    glm::mat4 m_modelMatrix;

    glm::mat4 m_viewMatrix;

    glm::vec4 m_color;

    GLint m_texLoc;
    GLint m_colorLoc;

    GLuint m_programs[4];
    GLuint m_curProgram;

    // Zero for TextureData, One - for Matrices.
    GLuint m_uniformPoints[2];
    GLuint m_uniformLinesAdj[2];
    GLuint m_uniformTriangles[2];
    GLuint m_uniformLines[2];

    GLuint m_vertexShader = 0;
    GLuint m_geomShaders[4];
    GLuint m_fragmentShader = 0;

    GLuint m_matricesUBO;
    GLuint m_textureDataUBO;

    int m_isTextureRender;

    /**
     * Utility cleanup functions
     */
    void remove_programs();
    void remove_shaders();
    void free_buffers();
};

#endif //MOONLANDER_MOONLANDERPROGRAM_HPP
