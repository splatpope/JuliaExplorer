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
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

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
// BRO WE NEED TO ABSTRACT THAT SHIET

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
    // ^^^ test this once we get rendertarget stuff figured out

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
    // ^^^ useless unless we want to zoom with the mouse (likely) or make a gui with scrollbars (very likely)

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    // but really is this useful ?

    //glEnable(GL_MULTISAMPLE);
/////////////////////////////////////THIS IS WHERE THE REAL FUN BEGINS////////////////////////////////////////////////////////////

    // flipping cause gl is dumb, but heck we use a 1D texture
    stbi_set_flip_vertically_on_load(true);

    //THIS BLOCK MAKES SURE EVERYTHING IS DESTROYED PROPERLY
    {
        check_gl_error(); //paranoia

/*      
        //BEFORE THIS CAN WORK, WE NEED TO UPGRADE Julia::
        //
        //init screen and its shader
        Screen viewport(SCR_WIDTH/SCR_HEIGHT, SCR_VIEWPORT_SIZE_FACTOR, SCR_VIEWPORT);
        Shader vpShader("src/textured.vs", "src/textured.fs");
        //^^^ THIS SHOULD BE IN Julia::
*/
        //init julia object
        Julia jl(window, "asset/redgold.png");
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

            // all this just to get a fps counter, pfft...
            double currentTime = glfwGetTime();
            deltaTime = currentTime - lastFrame;
            lastFrame = currentTime;
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ) // if at least 1 second elapsed 
            { 
                // WHAT KIND OF ASSHOLES MAKES A C TEXT LIBRARY THAT ONLY ACCEPS C STRINGS wait
                std::string frames = std::to_string(nbFrames);
                // and update gltext
                gltSetText(fps_counter,  (frames + " fps").c_str()); // I MEAN COME ON
                // reset timer for the next frame count
                nbFrames = 0;
                lastTime += 1.0;
            }
            processInput(window, jl);

            //BEGIN RENDERING
            glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT); //no need for depth

            //should be all we need
            jl.Draw();

            gltSetText(debug, jl.Report().c_str());
            check_gl_error();
            // why isn't there a standard way of displaying text in opengl ?
            gltBeginDraw();
            gltColor(1.0f, 1.0f, 1.0f, 1.0f);
            gltDrawText2D(fps_counter, SCR_WIDTH - 50, SCR_HEIGHT - 25, 1.0f); // x=0.0, y=0.0, scale=1.0
            gltDrawText2D(debug, SCR_WIDTH * SCR_VIEWPORT_SIZE_FACTOR + 5, 10, 1.0f);
            // ^^^ gonna need to update this when viewport stuff is deglobalized
            gltEndDraw();

            // FINALLY WE ARE DONE
            glfwSwapBuffers(window);
            glfwPollEvents();
            check_gl_error();

        }
        check_gl_error();
        gltDeleteText(fps_counter); 
        gltTerminate();

    }// Julia instance should be deleted at this point

    glfwTerminate();

    std::cout << "END OF PROGRAM, GG YOU DIDNT SEGFAULT" << std::endl; // but DID WE ???


    return 0;
}
//////////////////////////////////////////END OF MAIN////////////////////////////////////////////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // this is clearly not sufficient, we just need to notify the julia instance that 
    // its framebuffers changed sizes (with all the crap that entails)
    glViewport(0, 0, width , height);
    JULIA_FRAMEBUFFER_DIRTY = true;
}

void processInput(GLFWwindow *window, Julia &app)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // TODO: put all this in an input handler, I guess
    // (btw it should support key<->codepoints bindings and just need to be notified here, instead of manually checking if pressed)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        app.Pan(CAM_FORWARD); 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        app.Pan(CAM_BACKWARD); 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        app.Pan(CAM_LEFT); 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        app.Pan(CAM_RIGHT); 
    //get dir keys to change fractal offset
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        app.MoveOffset(CAM_FORWARD);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        app.MoveOffset(CAM_BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        app.MoveOffset(CAM_RIGHT);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        app.MoveOffset(CAM_LEFT);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        app.MoveOffset(CAM_RESET); 
    //zoom with + and -
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        app.Zoom(CAM_FORWARD); 
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        app.Zoom(CAM_BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        std::cout<<"lol";

    // ideally this should be a scrollable bar on the **future** gui
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
        app.iter = 10;
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        app.iter = 100;
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
        app.iter = 1000;

    // TODO: figure out how to write a png to disk from a gl texture -> done :))
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // HOPEFULLY DOESNT TRIGGER EVERY FRAME
        app.queue_screenshot = true;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
// how this is gonna help us, I don't know yet, but I sense it will
//
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // this is in screen coordinates, y axis goes down

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset); //nope
}


/* // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
} */