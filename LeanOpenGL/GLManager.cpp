#include "GLManager.h"
#include "GLCommon.h"

using namespace std;
using namespace gl;
using namespace gli;

GLManager::GLManager(GLRenderer& renderer)
    : _renderer(renderer)
{
}

GLManager::~GLManager()
{
}

void GLManager::createGrid(int width, int height, float step_x, float step_y)
{
    for (int i = 0; i <= width - 1; i++)
    {
        for (int j = 0; j <= height - 1; j++)
        {
            patch_vector.push_back(i*step_x);
            patch_vector.push_back(j*step_y);

            patch_vector.push_back(i*step_x);
            patch_vector.push_back(j*step_y + step_y);

            patch_vector.push_back(i*step_x + step_x);
            patch_vector.push_back(j*step_y);

            patch_vector.push_back(i*step_x + step_x);
            patch_vector.push_back(j*step_y + step_y);
        }
    }

    _renderer.addObject(patch_vector.size() * sizeof(float), patch_vector.data(), GL_PATCHES, 2);
}

void GLManager::loadTexture(std::string filename)
{
    texture2d ground_texture(load(filename.c_str()));

    if (ground_texture.empty())
        return;
}

void GLManager::loadHeights(std::string filename)
{
    texture2d ground_heights(load(filename.c_str()));

    if (ground_heights.empty())
        return;
}

void __stdcall GLManager::shader_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
    unsigned int source_i = source - 0x8246;
    unsigned int type_i = type - 0x824C;

    // Severity message correction, see https://www.opengl.org/registry/specs/KHR/debug.txt for explanation on severity tokens
    unsigned int severity_i = severity >= 0x9146 ? severity - 0x9146 : severity - 0x826B + 3;

    stringstream ss;
    ss << source_str[source_i] << type_str[type_i] << id << " " << severity_str[severity_i]
        << "Message length: " << length << " Message: " << message << " " << (user_param != nullptr ? *(char*)user_param : *(char*)" ") << endl;

    GLLogger _logger;

    _logger.regular_log(ss.str().c_str());
}

void GLManager::glfw_error_callback(int error, const char* description)
{
    stringstream ss;

    ss << "GLFW ERROR: code " << error << " msg: " << description << endl;

    GLLogger _logger;

    _logger.error_log(ss.str().c_str());
}

void GLManager::update_fps_counter(GLFWwindow* window)
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
