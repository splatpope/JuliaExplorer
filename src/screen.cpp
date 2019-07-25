#include "screen.h"

void Screen::Setup(const uint scr_type)
{

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
    };
    //Define a square
    Vertex tl, tr, bl, br;

    switch(scr_type)
    {
        case(SCR_RENDERTARGET):
            {
                float boundary = m_aspect_ratio * m_rel_size;
                //top left
                tl.Position = glm::vec2{-1, 1};
                tl.TexCoord = glm::vec2{-boundary, 1};
                //top right
                tr.Position = glm::vec2{1, 1};
                tr.TexCoord = glm::vec2{boundary, 1};
                //bottom left
                bl.Position = glm::vec2{-1, -1};
                bl.TexCoord = glm::vec2{-boundary, -1};
                //bottom right
                br.Position = glm::vec2{1, -1};
                br.TexCoord = glm::vec2{boundary, -1};
                break;
            }
        case(SCR_VIEWPORT):
            {
                float margin = 2 * m_rel_size - 1;
                //top left
                tl.Position = glm::vec2{-1, 1};
                tl.TexCoord = glm::vec2{0, 1};
                //top right
                tr.Position = glm::vec2{margin, 1};
                tr.TexCoord = glm::vec2{1, 1};
                //bottom left
                bl.Position = glm::vec2{-1, -1};
                bl.TexCoord = glm::vec2{0, 0};
                //bottom right
                br.Position = glm::vec2{margin, -1};
                br.TexCoord = glm::vec2{1, 0};
                break;
            }
    }

    std::vector<Vertex> verts{tl, tr, br, br, bl, tl};

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << "Initializing Screen VBO with name : "<< VBO << std::endl;
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec2));

    glBindVertexArray(0);
    check_gl_error();
}

void Screen::Draw()
{
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

std::string Screen::Report()
{
    std::string _report;
    std::sprintf(&_report[0], "COOL");
    return _report;
}

// because the user should be able to control binding, thanks opengl wiki I guess
void Screen::Bind()
{
    glBindVertexArray(VAO);
}

void Screen::UnBind()
{
    glBindVertexArray(0);
}