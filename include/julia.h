#ifndef JULIA_H_INCLUDED
#define JULIA_H_INCLUDED

#include "glad/glad.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <string>

#include <memory>
#include "screen.h"
#include "shader.h"
#include "camera.h"

class Julia
{
public:
    GLuint Palette;
    glm::vec2 Offset;
    glm::vec2 PanDir;
    float ZoomFactor;
    float ratio;
    bool dirty;
    float rel_size;
    float MoveIncrements;
    float ZoomIncrements;

    Julia(std::string src_img) :
    Offset(),
    PanDir(),
    ZoomFactor(0.9f),
    MoveIncrements(0.005*ZoomFactor),
    ZoomIncrements(MoveIncrements),
    rel_size(0.75f),
    m_plane(rel_size)
    {    
        m_shader = new Shader("src/julia.vs", "src/julia.fs");
        //Create palette texture and set it up
        initPalette(src_img);
        dirty = true;
    }
    ~Julia()
    {
        delete(m_shader);
        glDeleteTextures(1, &Palette);
        util::EmitLine("Fractal cleanup complete.");
    }

    void UseShader();
    void Update();
    void MoveOffset(Camera_Movement dir);
    void Pan(Camera_Movement dir);
    void Zoom(Camera_Movement dir);
    std::string Report();
    void Draw();
private:
    Screen m_plane;
    Shader * m_shader;

    void initPalette(std::string src_path);
};

#endif