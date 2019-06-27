#include "screen.h"

void Screen::Setup(const float & _rel_size)
{
    m_rel_size = _rel_size;

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
    };
    //Define a square
    Vertex tl, tr, bl, br;

    //top left
    tl.Position = glm::vec2{-1, 1};
    tl.TexCoord = glm::vec2{-1, 1};
    //top right
    tr.Position = glm::vec2{1-m_rel_size, 1};
    tr.TexCoord = glm::vec2{1, 1};
    //bottom left
    bl.Position = glm::vec2{-1, -1};
    bl.TexCoord = glm::vec2{-1, -1};
    //bottom right
    br.Position = glm::vec2{1-m_rel_size, -1};
    br.TexCoord = glm::vec2{1, -1};

    std::vector<Vertex> verts{tl, tr, br, br, bl, tl};

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << VBO << std::endl;
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
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

std::string Screen::Report()
{
    std::string _report;
    std::sprintf(&_report[0], "COOL");
    return _report;
}

void Screen::Bind()
{
    glBindVertexArray(VAO);
}

void Screen::UnBind()
{
    glBindVertexArray(0);
}