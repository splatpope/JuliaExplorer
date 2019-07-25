#ifndef JULIA_H_INCLUDED
#define JULIA_H_INCLUDED

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#endif
#endif

#include <string>

#include <memory>
#include "screen.h"
#include "shader.h"
#include "camera.h"

#define SCR_BORDER_WIDTH 3
#define SCR_WIDTH 1200
#define SCR_HEIGHT 600
#define SCR_VIEWPORT_SIZE_FACTOR 0.75f


class Julia
{
public:
    GLuint Palette;
    int iter;

    glm::vec2 Offset;
    glm::vec2 PanDir;
    float ZoomFactor;
    bool dirty;
    bool queue_screenshot;
    float MoveIncrements;
    float ZoomIncrements;

    Julia(GLFWwindow *window, std::string src_img) :
    iter(500),
    Offset(),
    PanDir(),
    ZoomFactor(0.75f),
    MoveIncrements(0.01f),
    ZoomIncrements(0.05f)
    {   
        glm::vec2 _vp = getViewport(window);
        std::cout << _vp.x << " " << _vp.y << std::endl;
        m_plane = new Screen(_vp.x / _vp.y, SCR_VIEWPORT_SIZE_FACTOR, SCR_RENDERTARGET); // WHAT DO YOU MEAN WE SHOULD REPLACE THOSE GLOBALS ??
        m_viewport = new Screen(_vp.x / _vp.y, SCR_VIEWPORT_SIZE_FACTOR, SCR_VIEWPORT);
        m_shader = new Shader("src/julia.vs", "src/julia.fs");
        m_vpShader = new Shader("src/textured.vs", "src/textured.fs");
        //Create palette texture and set it up
        initPalette(src_img);
        dirty = true;
    }
    ~Julia()
    {
        delete(m_plane);
        delete(m_viewport);
        delete(m_shader);
        delete(m_vpShader);
        glDeleteTextures(1, &Palette);
        glDeleteTextures(1, &m_renderTarget);
        util::EmitLine("Julia cleanup complete.");
    }

    glm::vec2 getViewport( GLFWwindow * window )
    {
        int *w = new int();
        int *h = new int();
        glfwGetFramebufferSize(window, w, h);
        float width = float(*w);
        float height = float(*h);
        delete w;
        delete h;
        return glm::vec2(width, height);
    }

    void Update();
    void MoveOffset(Camera_Movement dir);
    void Pan(Camera_Movement dir);
    void Zoom(Camera_Movement dir);
    std::string Report();
    void Draw();
    int Screenshot();
private:
    GLuint m_fbo;
    GLuint m_renderTarget;
    Screen * m_plane;
    Screen * m_viewport;
    Shader * m_shader;
    Shader * m_vpShader;

    GLFWwindow * window;

    void initPalette(std::string src_path);
};

#endif