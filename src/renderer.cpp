#include "renderer.h"

Renderer::Renderer()
{
    /*
    GLuint vao;
    glGenVertexArrays(1, &vao);
    r_next.vao = vao;
    */
}
/* 
void Renderer::SetupAttribPointers()
{

    //Setup default attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    //Format is : XYZ UVW RGBA

    util::EmitLine("Vertex attributes have been setup");
}
*/
/*
//should generate a pass from existing VAO and generated VBO from model info
void Renderer::AddModel(Model&& m)
{
    models.push_back(std::move(m));

    glBindVertexArray(r_next.vao);

    util::DebugLog("Models list size : ", models.size());

    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
    SetupAttribPointers();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    util::EmitLine("model has been added");
    check_gl_error();
    util::DebugLog("MODEL ADDED : ", m.vbo);
}
*/
void Renderer::Draw(gl::Geometry & model)
{
    util::EmitLine(">>RENDERING BEGIN");
    /*
    glBindVertexArray(next.vao);

    for ( auto v : model_list )
    {

        util::DebugLog("BEFORE DRAWING : ", models[v].vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[v].ebo);
        glDrawElements(models[v].primitive_style, models[v].n_indices, GL_UNSIGNED_INT, 0);
        check_gl_error();
    }
/* 
    for ( std::vector<Model>::iterator i = models.begin(); i != models.end(); ++i)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i->ebo);
        glDrawElements(i->primitive_style, i->n_indices, GL_UNSIGNED_INT, 0);
    } */

    model.draw();
    util::EmitLine(">>RENDERING DONE");
}

void Renderer::Debug()
{
    //an attempt was made       
    util::EmitLine("///////////////Renderer Debug/////////");
    //util::DebugLog("RENDERER DEBUG : VAO index : ", r_next.vao);
    util::EmitLine("///////////////////////////////////////");
}