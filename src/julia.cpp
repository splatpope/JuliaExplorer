#include "julia.h"
#include "util.h"
#include <vector>

void Julia::initPalette(std::string src_path)
{
    //generate and bind render target fbo
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    check_gl_error();

    //create render target texture representing our complex plane
    glGenTextures(1, &m_renderTarget);
    glBindTexture(GL_TEXTURE_2D, m_renderTarget);
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(SCR_WIDTH * SCR_VIEWPORT_SIZE_FACTOR), 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //put scaledWidth and scr_height instead
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    check_gl_error();

    //attach render target to the fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTarget, 0); 
    check_gl_error();
    {
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
        if (fboStatus == GL_FRAMEBUFFER_UNSUPPORTED) 
            std::cout << "implementation is not supported by OpenGL driver " << fboStatus << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //create palette texture using the julia shader
    m_shader->use();
    glActiveTexture(GL_TEXTURE0);
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
    }
    else
    {
        std::cout << "FAILED TO LOAD IMAGE" << std::endl;
    }

    //setup uniforms
    glUniform1i(glGetUniformLocation(m_shader->ID, "tex"), 0);
    m_shader->setVec2("c", glm::vec2{});
    m_shader->setInt("iter", iter);
    m_shader->setVec2("pan", glm::vec2{});
    m_shader->setFloat("zoom", ZoomFactor);

    glBindTexture(GL_TEXTURE_1D, 0);
    stbi_image_free(data);
    check_gl_error();
}

//input functions
//they take a direction and increment variables accordingly, then mark the application for update
void Julia::MoveOffset(Camera_Movement dir)
{

    switch(dir)
    {
        case(CAM_FORWARD) : Offset += glm::vec2(0, MoveIncrements); break;
        case(CAM_BACKWARD) : Offset -= glm::vec2(0, MoveIncrements); break;
        case(CAM_LEFT) : Offset -= glm::vec2(MoveIncrements, 0); break;
        case(CAM_RIGHT) : Offset += glm::vec2(MoveIncrements, 0); break;
        default : Offset = glm::vec2();
    }
    this->dirty = true;
}

void Julia::Pan(Camera_Movement dir)
{
    switch(dir)
    {
        case(CAM_FORWARD) : PanDir += glm::vec2(0, MoveIncrements); break;
        case(CAM_BACKWARD) : PanDir -= glm::vec2(0, MoveIncrements); break;
        case(CAM_LEFT) : PanDir -= glm::vec2(MoveIncrements, 0); break;
        case(CAM_RIGHT) : PanDir += glm::vec2(MoveIncrements, 0); break;
        default : PanDir = glm::vec2();
    }
    this->dirty = true;
}

void Julia::Zoom(Camera_Movement dir)
{
    float strength = 0.5f;
    switch(dir)
    {
        case(CAM_FORWARD) : ZoomFactor += ZoomIncrements * ZoomFactor; break;
        case(CAM_BACKWARD) : ZoomFactor -= ZoomIncrements * ZoomFactor; break;
    }
    this->dirty = true;
}

//this serves as debugging information to be rendered by gltext
std::string Julia::Report()
{
    std::string _report;
    _report += "Offset : "+glm::to_string(Offset)+"\n";
    _report += "Pan : "+glm::to_string(PanDir)+"\n";
    _report += "ZoomFactor : "+std::to_string(ZoomFactor)+"\n";
    _report += "MoveIncrements : "+std::to_string(MoveIncrements)+"\n";
    _report += "ZoomIncrements : "+std::to_string(ZoomIncrements)+"\n";
    _report += "Iterations : "+std::to_string(iter)+"\n";

    return _report;
}

void Julia::Screenshot()
{
    std::cout<<"this is supposed to be a screenshot"<<std::endl;
    //soon

}

void Julia::Update()
{
    glActiveTexture(GL_TEXTURE0); //overkill but hey
    m_shader->use();

    if(dirty == true) //TODO: actually implement input event validation
    {
        m_shader->setVec2("c", Offset);
        m_shader->setVec2("pan", PanDir);
        m_shader->setFloat("zoom", ZoomFactor);
        //m_shader->setFloat("rel_size", 0.75f);
        m_shader->setInt("iter", iter);

        MoveIncrements = 0.01 * log(ZoomFactor * 10.0f) / ZoomFactor;

        dirty = false;
  
    }
    check_gl_error();


    glBindTexture(GL_TEXTURE_1D, Palette);
    check_gl_error();
}

void Julia::Draw()
{
    //render fractal to render target fbo
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, static_cast<int>(SCR_WIDTH * SCR_VIEWPORT_SIZE_FACTOR), SCR_HEIGHT);
    Update();
    m_plane->Bind();
    m_plane->Draw();
    m_plane->UnBind();

    //then display it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    m_vpShader->use();
    glBindTexture(GL_TEXTURE_2D, m_renderTarget);
    m_viewport->Bind();
    m_viewport->Draw();
    m_viewport->UnBind();

}