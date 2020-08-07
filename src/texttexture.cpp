#include <GL/glew.h>
#include <texttexture.h>
#include <string>
#include <SDL_ttf.h>

void TextTexture::setText(const std::string& text_)
{
    load(text_, color, font);
    text = text_;
}

void TextTexture::setFont(TTF_Font* font_)
{
    this->load(text, color, font_);
    font = font_;
}

void TextTexture::setColor(SDL_Color color_)
{
    this->load(text, color_, font);
    color = color_;
}

TextTexture::TextTexture(const std::string &textureText, SDL_Color color_, TTF_Font *font_)
{
    font = font_;
    color = color_;
    text = textureText;
    vbo_id = ibo_id = vao_id = 0;
    textureID = 0;
    load(textureText, color, font);
}

bool TextTexture::load(const std::string &textureText, SDL_Color color_, TTF_Font* font_)
{
    freeTexture();
    SDL_Surface* surface = TTF_RenderText_Blended(font_, textureText.c_str(), color_);

    surface = Utils::flipVertically(surface);
    if (!surface) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        // get the number of channels in the SDL surface
        GLenum texture_format;
        GLint  nOfColors;
        nOfColors = surface->format->BytesPerPixel;
        if( nOfColors == 4 )     // contains an alpha channel
        {
            if(surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            else
                texture_format = GL_BGRA;
        }
        else if( nOfColors == 3 )     // no alpha channel
        {
            if(surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            else
                texture_format = GL_BGR;
        }
        else
        {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }
        texture_width = surface->w;
        texture_height = surface->h;

        textureID = Utils::loadTextureFromPixels32(static_cast<GLuint*>(surface->pixels),
                                                   texture_width, texture_height, nOfColors, texture_format);
        SDL_FreeSurface(surface);

        initVBO();
    }
}

void TextTexture::initVBO()
{
    if (textureID != 0 && vbo_id == 0) {
        GLfloat quadWidth = texture_width;
        GLfloat quadHeight = texture_height;

        GLfloat vertices[] = {
                // positions            // texture coords
                quadWidth,  0.0f,       1.0f, 1.0f, // top right
                quadWidth, quadHeight,  1.0f, 0.0f, // bottom right
                0.f, quadHeight,        0.0f, 0.0f, // bottom left
                0.f,  0.f,              0.0f, 1.0f  // top left
        };

        GLuint indices[] = {
                3, 1, 0,
                3, 2, 1
        };

        glGenVertexArrays(1, &vao_id);
        glGenBuffers(1, &vbo_id);
        glGenBuffers(1, &ibo_id);

        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        //position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        //Tex coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void TextTexture::freeVBO()
{
    if (vbo_id != 0) {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ibo_id);
    }
}


void TextTexture::render(MoonLanderProgram& program, GLfloat x,
                         GLfloat y, Utils::Rect* clip, double angle)
{
    GLuint clip_w = texture_width;
    GLuint clip_h = texture_height;
    if (clip != nullptr) {
        clip_w = clip->w;
        clip_h = clip->h;
    }

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTranslatef(x + clip_w / 2.f, y + clip_h / 2.f, 0);
    glRotatef(angle, 0.f, 0.f, 1.f);
    glTranslatef(-(x + clip_w / 2.f), -(y + clip_h / 2.f), 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    render(program, x, y, clip);
    glPopMatrix();
}

void TextTexture::render(MoonLanderProgram& program, GLfloat x,
                         GLfloat y, Utils::Rect* clip)
{
    if (textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        program.setTexture(0);

        program.setView(glm::translate(program.getView(), glm::vec3(x, y, 0.f)));
        program.updateView();

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        program.setView(glm::translate(program.getView(), glm::vec3(-x, -y, 0.f)));
        program.updateView();

        glBindVertexArray(0);
    }
}

void TextTexture::freeTexture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    texture_width = texture_height = 0;
}

TextTexture::~TextTexture()
{
    freeTexture();
    freeVBO();
}
