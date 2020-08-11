#include <GL/glew.h>
#include <sprite.hpp>
#include <SDL_image.h>

Sprite::Sprite(const std::string& path)
{
    load(path);
    VAO = nullptr;
    tot_sprites = 0;
}

bool Sprite::load(const std::string& path)
{
    if (!path.empty()) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        surface = Utils::flipVertically(surface);
        if (surface == nullptr) {
            SDL_Log("LoadTexture: %s\n", SDL_GetError());
            std::abort();
        } else {
            GLenum texture_format = Utils::getSurfaceFormatInfo(*surface);

            texture_width = surface->w;
            texture_height = surface->h;

            textureID = Utils::loadTextureFromPixels32(static_cast<GLuint*>(surface->pixels),
                                                       texture_width, texture_height, texture_format);
            SDL_FreeSurface(surface);
        }
    }

    return true;
}

GLuint Sprite::addClipSprite(Utils::Rect clip)
{
    clips.push_back(clip);
    return clips.size() - 1;
}

Utils::Rect Sprite::getClip(int idx)
{
    return clips[idx];
}

bool Sprite::generateDataBuffer()
{
    if (textureID != 0 && !clips.empty()) {
        tot_sprites = clips.size();
        VAO = new GLuint[tot_sprites];

        glGenVertexArrays(tot_sprites, VAO);
        GLuint VBO;
        GLuint EBO;
        GLfloat tw = texture_width;
        GLfloat th = texture_height;

        GLuint indices[] = {
                3, 1, 0,
                3, 2, 1
        };

        // 8 Vertex coordinates + 8 texture coordinates
        GLfloat vertices[16];

        vertices[1] = 0.0f;
        vertices[8] = 0.f;
        vertices[12] = 0.f;
        vertices[13] = 0.f;

        for (GLuint i = 0; i < tot_sprites; ++i) {
            vertices[0] = clips[i].w;
            vertices[2] = (clips[i].x + clips[i].w) / tw;
            vertices[3] = (clips[i].y + clips[i].h) / th;
            vertices[4] = clips[i].w;
            vertices[5] = clips[i].h;
            vertices[6] = (clips[i].x + clips[i].w) / tw;
            vertices[7] = clips[i].y / th;
            vertices[9] = clips[i].h;
            vertices[10] = clips[i].x / tw;
            vertices[11] = clips[i].y / th;
            vertices[14] = clips[i].x / tw;
            vertices[15] = (clips[i].y + clips[i].h) / th;

            glBindVertexArray(VAO[i]);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glDeleteBuffers(1, &EBO);
            glDeleteBuffers(1, &VBO);
        }

    } else {
        if (textureID == 0) {
            SDL_Log("No texture to render!\n");
            std::abort();
        }

        if (clips.empty()) {
            SDL_Log("No data generate from!\n");
            std::abort();
        }
    }

    return true;
}

void Sprite::freeSheet()
{
    if (VAO != nullptr) {
        glDeleteBuffers(tot_sprites, VAO);
        VAO = nullptr;
    }

    clips.clear();
}

void Sprite::render(MoonLanderProgram& program, GLfloat x, GLfloat y, GLuint idx, GLfloat angle)
{
    if (VAO != nullptr) {
        glm::mat4 tr1 = glm::translate(glm::mat4(1.f), glm::vec3(x + clips[idx].w / 2.f, y + clips[idx].h / 2.f, 0.f));
        glm::mat4 ro = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 tr2 = glm::translate(glm::mat4(1.f), glm::vec3(-clips[idx].w / 2.f, -clips[idx].h / 2.f, 0.f));
        glm::mat4 transform = tr1 * ro * tr2;

        program.leftMultModel(transform);
        program.updateModel();

        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(VAO[idx]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        tr1 = glm::translate(glm::mat4(1.f), glm::vec3(clips[idx].w / 2.f, clips[idx].h / 2.f, 0.f));
        ro = glm::rotate(glm::mat4(1.f), -angle, glm::vec3(0.f, 0.f, 1.f));
        tr2 = glm::translate(glm::mat4(1.f), glm::vec3(-(x + clips[idx].w / 2.f), -(y + clips[idx].h / 2.f), 0.f));
        transform = tr1 * ro * tr2;

        program.leftMultModel(transform);
        program.updateModel();
    }
}

Sprite::~Sprite()
{
    freeSheet();
}

