#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <vector>
#include <functional>
#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "util.h"

class Screen
{
public:
    Screen()
    {
        Screen(0.75f);
    }
    Screen(float _rel_size)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        Setup(_rel_size);
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
    float m_rel_size;

    void Setup(const float &_rel_size);

};
#endif