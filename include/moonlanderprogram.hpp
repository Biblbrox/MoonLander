#ifndef MOONLANDER_MOONLANDERPROGRAM_HPP
#define MOONLANDER_MOONLANDERPROGRAM_HPP

#include <glm/mat4x4.hpp>
#include <memory>

#include "render/shaderprogram.hpp"

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

    /**
     * Write specific uniform to current program
     */
    void updateColor();
    void updateProjection() override;
    void updateView() override;
    void updateModel() override;

    /**
     * Init programs
     */
    void loadProgram() override;

private:
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
