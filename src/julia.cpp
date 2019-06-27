#include "julia.h"
#include "util.h"
#include <vector>

void Julia::initPalette(std::string src_path)
{
    UseShader();
    glActiveTexture(GL_TEXTURE0 + 1);
    glGenTextures(1, &Palette);
    glBindTexture(GL_TEXTURE_1D, Palette);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Load palette data
    unsigned char *data;
    int width, height, nrChannels;
    data = stbi_load(src_path.c_str(), &width, &height, &nrChannels, STBI_rgb);
    if(data)
    {
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        check_gl_error();
        //glGenerateMipmap(GL_TEXTURE_1D);
    }
    else
    {
        std::cout << "FAILED TO LOAD IMAGE" << std::endl;
    }

    glUniform1i(glGetUniformLocation(m_shader->ID, "tex"), 1);
    m_shader->setVec2("c", glm::vec2{});
    m_shader->setInt("iter", 1000);
    m_shader->setVec2("pan", glm::vec2{});
    m_shader->setFloat("zoom", 1.0f);
    m_shader->setFloat("ratio", ratio);
    m_shader->setFloat("rel_size", rel_size);

    glBindTexture(GL_TEXTURE_1D, 0);
    stbi_image_free(data);
    check_gl_error();
}

void Julia::UseShader()
    {
        m_shader->use();
    }

void Julia::Update()
{
    UseShader();
    glActiveTexture(GL_TEXTURE0 + 1);

    if(dirty == true)
    {
        m_shader->setVec2("c", Offset);
        m_shader->setVec2("pan", PanDir);
        m_shader->setFloat("zoom", ZoomFactor);
        m_shader->setFloat("ratio", ratio);
        m_shader->setFloat("rel_size", 0.75f);

        MoveIncrements = 0.005/ZoomFactor;

        dirty = false;
  
    }
    check_gl_error();


    glBindTexture(GL_TEXTURE_1D, Palette);
    check_gl_error();
}

void Julia::MoveOffset(Camera_Movement dir)
{

    switch(dir)
    {
        case(FORWARD) : Offset += glm::vec2(0, MoveIncrements); break;
        case(BACKWARD) : Offset -= glm::vec2(0, MoveIncrements); break;
        case(LEFT) : Offset -= glm::vec2(MoveIncrements, 0); break;
        case(RIGHT) : Offset += glm::vec2(MoveIncrements, 0); break;
        default : Offset = glm::vec2();
    }
    this->dirty = true;
}

void Julia::Pan(Camera_Movement dir)
{
    switch(dir)
    {
        case(FORWARD) : PanDir += glm::vec2(0, MoveIncrements); break;
        case(BACKWARD) : PanDir -= glm::vec2(0, MoveIncrements); break;
        case(LEFT) : PanDir -= glm::vec2(MoveIncrements, 0); break;
        case(RIGHT) : PanDir += glm::vec2(MoveIncrements, 0); break;
        default : PanDir = glm::vec2();
    }
    this->dirty = true;
}

void Julia::Zoom(Camera_Movement dir)
{

    switch(dir)
    {
        case(FORWARD) : ZoomFactor += ZoomIncrements; break;
        case(BACKWARD) : ZoomFactor -= ZoomIncrements; break;
    }
    this->dirty = true;
}

std::string Julia::Report()
{
    std::string _report = "Offset : "+glm::to_string(Offset)+"\n";
    _report += "Pan : "+glm::to_string(PanDir)+"\n";
    _report += "ZoomFactor : "+std::to_string(ZoomFactor)+"\n";
    _report += "MoveIncrements : "+std::to_string(MoveIncrements)+"\n";
    _report += "ZoomIncrements : "+std::to_string(ZoomIncrements)+"\n";

    return _report;
}

void Julia::Draw()
{
    Update();
    m_plane.Bind();
    m_plane.Draw();
    m_plane.UnBind();
}