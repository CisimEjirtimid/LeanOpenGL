
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

int main()
{
    GLLogger _logger;

    _logger.time_log();

    glfwSetErrorCallback(GLManager::glfw_error_callback);

    if (!glfwInit())
    {
        _logger.error_log("ERROR: Could not start GLFW");
        return -1;
    }

    // Setting up window hints just before it's creation
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
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

    if (GLEW_KHR_debug)
    {
        const char user_param[] = "Dimitrije";

        _logger.regular_log("KHR_debug extension found");
        glDebugMessageCallback(GLManager::shader_debug_callback, user_param);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        _logger.regular_log("Debug callback engaged");
    }
    else
    {
        _logger.error_log("EROOR: KHR_debug extension not found");
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    //  conversion from ubyte* (char* and so) to string
    //string renderer_str(reinterpret_cast<char const*>(renderer));

    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported: " << version << endl;

#pragma region Program Body
    GLRenderer _renderer;

    GLManager _manager(_renderer);
    glfwSetKeyCallback(window, _manager.glfw_keyboard_callback);

    _manager.create_grid(50, 50, 0.5f, 0.5f);

    _manager.load_texture("./data/n42_e022_1arc_v3.dds");
    //_manager.load_heights("./data/n42_e022_3arc_v2_raw.dds");
    _manager.load_heights("./data/n42_e022_3arc_v2.bil", 1201, 1201);


    _renderer.addShader(GL_VERTEX_SHADER, "./shaders/minimal.vert");
    _renderer.addShader(GL_FRAGMENT_SHADER, "./shaders/minimal.frag");
    _renderer.addShader(GL_TESS_CONTROL_SHADER, "./shaders/minimal.tesc");
    _renderer.addShader(GL_TESS_EVALUATION_SHADER, "./shaders/minimal.tese");

    _renderer.finishInit();

#pragma region Render Loop
    while (!glfwWindowShouldClose(window))
    {
        GLManager::update_fps_counter(window);

        _renderer.draw();

        glfwPollEvents();

        glfwSwapBuffers(window);
    }
#pragma endregion

#pragma endregion
    glfwTerminate();
    return 0;
}
