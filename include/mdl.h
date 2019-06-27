#ifndef MDL_H_INCLUDED
#define MDL_H_INCLUDED

#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "util.h"

namespace gl{
    enum Attribute
    {
        Position,
        Normal,
        Color,
        TexCoord0
    };
}

//default vertex format, should implement variable ones
struct Vertex 
{
    //Vertex attributes, must coincide with glVertexAttribPointer calls
    glm::vec3 Position {};
    glm::vec3 Normals {};
    glm::vec3 Color {};
    glm::vec2 TexCoords {};
};

class Mesh 
{
public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
};

class Model 
{
public:
    std::vector<Mesh> meshes;
    GLuint vbo;
    GLuint ebo;
    uint primitive_style = GL_TRIANGLES;
    uint n_indices;

    Model() { glGenBuffers(1, &vbo); glGenBuffers(1, &ebo); }
    Model(Model&& other)
    : vbo()
    , ebo()
    {
        vbo = other.vbo;
        ebo = other.ebo;
    }

    ~Model()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo); 
        util::EmitLine("model destroyed");
    }

    void FillBuffers();
    
};
#endif