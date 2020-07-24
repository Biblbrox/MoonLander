#include <GL/glew.h>
#include <sprite.h>
#include <SDL_image.h>
#include <GL/gl.h>

Sprite::Sprite(const std::string& path)
{
    load(path);
    vertexDataBuffer = 0;
    indexBuffers = nullptr;
}


bool Sprite::load(const std::string& path)
{
    if (!path.empty()) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface == nullptr) {
            SDL_Log("LoadTexture: %s\n", SDL_GetError());
            std::abort();
        }

        texture_width = surface->w;
        texture_height = surface->h;
        textureID = Utils::loadTextureFromPixels32(static_cast<GLuint*>(surface->pixels),
                                                   texture_width, texture_height);
        SDL_FreeSurface(surface);
    }
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
        int tot_sprites = clips.size();
        auto vertex_data = new Utils::VertexData2D[tot_sprites * 4];
        indexBuffers = new GLuint[tot_sprites];

        glGenBuffers(1, &vertexDataBuffer);
        glGenBuffers(tot_sprites, indexBuffers);
        GLfloat tw = texture_width;
        GLfloat th = texture_height;
        GLuint spriteIndices[4]{0, 0, 0, 0};

        for (int i = 0; i < tot_sprites; ++i) {
            spriteIndices[0] = i * 4 + 0;
            spriteIndices[1] = i * 4 + 1;
            spriteIndices[2] = i * 4 + 2;
            spriteIndices[3] = i * 4 + 3;

            vertex_data[spriteIndices[0]].position.x = 0;
            vertex_data[spriteIndices[0]].position.y = 0;
            vertex_data[spriteIndices[0]].texCoord.x = clips[i].x / tw;
            vertex_data[spriteIndices[0]].texCoord.y = clips[i].y / th;

            vertex_data[spriteIndices[1]].position.x = clips[i].w;
            vertex_data[spriteIndices[1]].position.y = 0;
            vertex_data[spriteIndices[1]].texCoord.x = (clips[i].x + clips[i].w) / tw;
            vertex_data[spriteIndices[1]].texCoord.y = clips[i].y / th;

            vertex_data[spriteIndices[2]].position.x = clips[i].w;
            vertex_data[spriteIndices[2]].position.y = clips[i].h;
            vertex_data[spriteIndices[2]].texCoord.x = (clips[i].x + clips[i].w) / tw;
            vertex_data[spriteIndices[2]].texCoord.y = (clips[i].y + clips[i].h) / th;

            vertex_data[spriteIndices[3]].position.x = 0;
            vertex_data[spriteIndices[3]].position.y = clips[i].h;
            vertex_data[spriteIndices[3]].texCoord.x = clips[i].x / tw;
            vertex_data[spriteIndices[3]].texCoord.y = (clips[i].y + clips[i].h) / th;

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), spriteIndices, GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexDataBuffer);
        glBufferData(GL_ARRAY_BUFFER, tot_sprites * 4 * sizeof(Utils::VertexData2D),
                     vertex_data, GL_STATIC_DRAW);

        delete[] vertex_data;
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
}

void Sprite::freeSheet()
{
    if (vertexDataBuffer != 0) {
        glDeleteBuffers(1, &vertexDataBuffer);
        vertexDataBuffer = 0;
    }

    if (indexBuffers != nullptr) {
        glDeleteBuffers(clips.size(), indexBuffers);
        delete[] indexBuffers;
        indexBuffers = nullptr;
    }

    clips.clear();
}

void Sprite::freeTexture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    texture_width = texture_height = 0;
}

void Sprite::render(GLfloat x, GLfloat y, int idx, GLfloat angle)
{
    if (vertexDataBuffer != 0) {
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTranslatef(x + clips[idx].w / 2.f, y + clips[idx].h / 2.f, 0);
        glRotatef(angle, 0.f, 0.f, 1.f);
        glTranslatef(-(x + clips[idx].w / 2.f), -(y + clips[idx].h / 2.f), 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glTranslatef(x, y, 0.f);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, vertexDataBuffer);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Utils::VertexData2D),
                          (GLvoid*)offsetof(Utils::VertexData2D, texCoord));
        glVertexPointer(2, GL_FLOAT, sizeof(Utils::VertexData2D),
                          (GLvoid*)offsetof(Utils::VertexData2D, position));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[idx]);
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
    }
}

Sprite::~Sprite()
{
    freeTexture();
    freeSheet();
}

