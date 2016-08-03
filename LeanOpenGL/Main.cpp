
// STD/STL headers
#include <iostream>
#include <sstream>

// OpenGL headers
#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <gli/gli.hpp>

// Lean headers
#include "GLRenderer.h"
#include "GLLogger.h"
#include "GLManager.h"

using namespace std;
using namespace gl;

//  add other callback functions here
//  later, maybe define class for callback functions
void glfw_error_callback(int error, const char* description)
{
    stringstream ss;

    ss << "GLFW ERROR: code " << error << " msg: " << description << endl;

    GLLogger _logger;

    _logger.error_log(ss.str().c_str());
}

void update_fps_counter(GLFWwindow* window)
{
    static double previous_seconds = glfwGetTime();
    static int frame_count;

    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;

    if (elapsed_seconds > 0.033)
    {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;

        stringstream ss;

        ss << "Lean GL Manager - FPS: " << fps;

        glfwSetWindowTitle(window, ss.str().c_str());
    }

}

float* createGrid(int width, int height, float step_x, float step_y)
{
    float* patch = new float[width*height * 8];

    int k = 0;
    for (int i = 0; i <= width - 1; i++)
        for (int j = 0; j <= height - 1; j++)
        {
            patch[k++] = i*step_x;
            patch[k++] = j*step_y;

            patch[k++] = i*step_x;
            patch[k++] = j*step_y + step_y;

            patch[k++] = i*step_x + step_x;
            patch[k++] = j*step_y;

            patch[k++] = i*step_x + step_x;
            patch[k++] = j*step_y + step_y;
       }

    return patch;
}

int main()
{
    GLLogger _logger;
    
    _logger.time_log();

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        _logger.error_log("ERROR: Could not start GLFW");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lean GL Manager", nullptr, nullptr);

    if (!window)
    {
        _logger.error_log("ERROR: Could not open window with GLFW");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    //  conversion from ubyte* (char* and so) to string
    //string renderer_str(reinterpret_cast<char const*>(renderer));

    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported: " << version << endl;

#pragma region Program Body
    GLRenderer _renderer;
    
    GLManager _manager(_renderer);
    _manager.createGrid(5, 5, 0.1, 0.1);


    _renderer.addShader(GL_VERTEX_SHADER, "./shaders/minimal.vert");
    _renderer.addShader(GL_FRAGMENT_SHADER, "./shaders/minimal.frag");
    _renderer.addShader(GL_TESS_CONTROL_SHADER, "./shaders/minimal.tesc");
    _renderer.addShader(GL_TESS_EVALUATION_SHADER, "./shaders/minimal.tese");

    _renderer.finishInit();

#pragma region Render Loop
    while (!glfwWindowShouldClose(window))
    {
        update_fps_counter(window);

        _renderer.draw();

        glfwPollEvents();

        glfwSwapBuffers(window);
    }
#pragma endregion

#pragma endregion
    glfwTerminate();
    return 0;
}
