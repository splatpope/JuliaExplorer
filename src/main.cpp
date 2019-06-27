#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLT_IMPLEMENTATION
#include "gltext.h"

#include "util.h"
#include "camera.h"
#include "julia.h"

#define segcheck() std::cout << "did i segfault yet" << std::endl;

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Julia &app);

// settings
const uint SCR_BORDER_WIDTH = 3;
const uint SCR_WIDTH = 1200;
const uint SCR_HEIGHT = 600;
bool JULIA_FRAMEBUFFER_DIRTY = true;

// mouse processing
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "KEDISS", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    //glEnable(GL_MULTISAMPLE);
/////////////////////////////////////THIS IS WHERE THE REAL FUN BEGINS////////////////////////////////////////////////////////////
    //bad
    //glViewport( SCR_BORDER_WIDTH, SCR_BORDER_WIDTH, std::ceil(SCR_WIDTH * 3/4.0f) - SCR_BORDER_WIDTH, SCR_HEIGHT - SCR_BORDER_WIDTH * 2);

    //flipping cause gl is dumb, but heck we use a 1D texture
    stbi_set_flip_vertically_on_load(true);
//TEXTURES ARE NICE AND SHIT BUT AUTOMATE PLS/////////////////////////////////////////////////////////////////////

    //THIS BLOCK MAKES SURE EVERYTHING IS DESTROYED PROPERLY
    {
        check_gl_error();


        //^^^^ ALL OF THIS MODEL CREATION BULLSHIT COULD BE DONE AUTOMATICALLY

        Julia jl("asset/softgold.png");
        std::cout<<"PALETTE 0 : "<<jl.Palette<<std::endl;

        check_gl_error();

        double lastTime = glfwGetTime();
        int nbFrames = 0;
    
        if (!gltInit()) 
        {
            fprintf(stderr, "Failed to initialize glText\n");
            glfwTerminate();
            return EXIT_FAILURE;
        }

        GLTtext *fps_counter = gltCreateText();
        gltSetText(fps_counter, "LOADING...");
        GLTtext *debug = gltCreateText();

        check_gl_error();
//render loop/////////////////////////////////////////////////////////////////////////////////////////////////////
        while(!glfwWindowShouldClose(window))
        {
            check_gl_error();

            double currentTime = glfwGetTime();
            deltaTime = currentTime - lastFrame;
            lastFrame = currentTime;
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
                // printf (or not) and reset timer
                std::string frames = std::to_string(nbFrames);
                //printf("%s fps\n", frames.c_str());
                // and update gltext
                gltSetText(fps_counter,  (frames + " fps").c_str());
                nbFrames = 0;
                lastTime += 1.0;
            }
            processInput(window, jl);

            //DO NOT FORGET TO PUT RENDERING **AFTER** THIS lol
            glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (JULIA_FRAMEBUFFER_DIRTY)
            {
                GLint m_viewport[4];
                glGetIntegerv(GL_VIEWPORT, m_viewport);
                int w = m_viewport[2];
                int h = m_viewport[3];
                jl.ratio = m_viewport[2]/float(m_viewport[3]);
                jl.dirty = true;
            }
            jl.Draw();
            gltSetText(debug, jl.Report().c_str());
            check_gl_error();

            gltBeginDraw();
            gltColor(1.0f, 1.0f, 1.0f, 1.0f);
            gltDrawText2D(fps_counter, 0.0f, 0.0f, 1.0f); // x=0.0, y=0.0, scale=1.0
            gltDrawText2D(debug, SCR_WIDTH*5/8.0f + 5, 10, 1.0f);
            gltEndDraw();

            glfwSwapBuffers(window);
            glfwPollEvents();
            check_gl_error();

        }
        check_gl_error();
        gltDeleteText(fps_counter); 
        gltTerminate();
    }

    glfwTerminate();

    std::cout << "END OF PROGRAM, GG YOU DIDNT SEGFAULT" << std::endl;


    return 0;
}
//////////////////////////////////////////END OF MAIN//////////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width , height);
    JULIA_FRAMEBUFFER_DIRTY = true;
}

void processInput(GLFWwindow *window, Julia &app)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        app.Pan(FORWARD); 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        app.Pan(BACKWARD); 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        app.Pan(LEFT); 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        app.Pan(RIGHT); 
    //get dir keys to change fractal offset
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        app.MoveOffset(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        app.MoveOffset(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        app.MoveOffset(RIGHT);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        app.MoveOffset(LEFT);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        app.MoveOffset((Camera_Movement)-1); 
    //zoom with + and -
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        app.Zoom(FORWARD); 
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        app.Zoom(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        std::cout<<"lol";
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}


/* // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
} */