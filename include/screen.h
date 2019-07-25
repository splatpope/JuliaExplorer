#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <vector>
#include <functional>
#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "util.h"

const uint SCR_RENDERTARGET = 0;
const uint SCR_VIEWPORT = 1;


class Screen
{
public:
    float m_rel_size;
    float m_aspect_ratio;
    Screen(float _aspect_ratio, float _rel_size, const uint _scr_type)
    {
        m_rel_size = _rel_size;
        m_aspect_ratio = _aspect_ratio;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        Setup(_scr_type);
    }
    ~Screen()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        std::cout<<"Screen cleanup complete."<<std::endl;
    }
    void Draw();
    std::string Report();
    void Bind();
    void UnBind();
private:
    GLuint VAO;
    GLuint VBO;

    void Setup(const uint scr_type);

};
#endif