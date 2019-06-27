#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <memory>
#include <vector>
#include "util.h"
#include "mdl.h"
#include "gl/GlDebug.h"
#include "gl/GlGeometry.h"

class Renderer
{
public:
    /*
    struct Pass
    {
        GLuint vao;
    };
    Pass r_next;
    */

    std::vector<Model> models;
    std::vector<Shader> shaders;    
    
    Renderer();
    ~Renderer()
    {
        //glDeleteVertexArrays(1, &(r_next.vao));
        util::EmitLine("renderer destroyedd");
    };

    void Draw(gl::Geometry &);
    /* 
    void AddModel(Model&& m);
    void SetupAttribPointers();
    */

    void Debug();
    
};

#endif