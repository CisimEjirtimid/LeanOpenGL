#include "GLManager.h"
#include "GLCommon.h"

using namespace std;
using namespace gl;
using namespace gli;
using namespace glm;

GLManager::GLManager(GLRenderer& renderer)
    : _renderer(renderer)
{
}

GLManager::~GLManager()
{
}

void GLManager::create_grid(int width, int height, float step_x, float step_y)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            //  1st vertex
            _patch_vertices.push_back(i*step_x);
            _patch_vertices.push_back(j*step_y);

            _patch_tex_coords.push_back((float)i / (float)width);
            _patch_tex_coords.push_back((float)j / (float)height);

            //  2nd vertex
            _patch_vertices.push_back(i*step_x);
            _patch_vertices.push_back(j*step_y + step_y);

            _patch_tex_coords.push_back((float)i / (float)width);
            _patch_tex_coords.push_back((float)(j + 1) / (float)height);

            //  3rd vertex
            _patch_vertices.push_back(i*step_x + step_x);
            _patch_vertices.push_back(j*step_y);

            _patch_tex_coords.push_back((float)(i + 1) / (float)width);
            _patch_tex_coords.push_back((float)j / (float)height);

            //  4th vertex
            _patch_vertices.push_back(i*step_x + step_x);
            _patch_vertices.push_back(j*step_y + step_y);

            _patch_tex_coords.push_back((float)(i + 1) / (float)width);
            _patch_tex_coords.push_back((float)(j + 1) / (float)height);
        }
    }

    _renderer.add_object(_patch_vertices.size() * sizeof(float), _patch_vertices.data(), GL_PATCHES, 2);
    _renderer.add_tex_coords(_patch_vertices.size() * sizeof(float), _patch_tex_coords.data()); // adding tex_coords, although i do not use them anywhere
}

void GLManager::load_texture(std::string filename)
{
    _renderer.load_texture(filename);
}

void GLManager::load_heights(std::string filename, unsigned width, unsigned height)
{
    _renderer.load_heights(filename, width, height);
}


void __stdcall GLManager::shader_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
    unsigned int source_i = source - 0x8246;
    unsigned int type_i = type - 0x824C;

    // Severity message correction, see https://www.opengl.org/registry/specs/KHR/debug.txt for explanation on severity tokens
    unsigned int severity_i = severity >= 0x9146 ? severity - 0x9146 : severity - 0x826B + 3;

    if (severity_i == 3) // Severity == NOTIFICATION
        return;

    stringstream ss;
    ss << source_str[source_i] << endl << type_str[type_i] << endl << "ID: " << id << endl << severity_str[severity_i] << endl
        << "Message: " << message << endl; // << (user_param != nullptr ? *(char*)user_param : *(char*)" ") << endl;

    GLLogger _logger;

    _logger.regular_log(ss.str().c_str());
}

void GLManager::glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        switch (key)
        {
            // Tesselation control
        case GLFW_KEY_KP_ADD:
            GLRenderer::increase_tesc_outer();
            GLRenderer::increase_tesc_inner();
            break;
        case GLFW_KEY_KP_SUBTRACT:
            GLRenderer::decrease_tesc_outer();
            GLRenderer::decrease_tesc_inner();
            break;

            // Wireframe control
        case GLFW_KEY_TAB:
            GLRenderer::invert_wireframe();
            break;

            // Translation
        case GLFW_KEY_W:
            GLRenderer::translate_camera(MOVE_FORWARD);
            break;
        case GLFW_KEY_S:
            GLRenderer::translate_camera(MOVE_BACKWARD);
            break;
        case GLFW_KEY_A:
            GLRenderer::translate_camera(STRAFE_LEFT);
            break;
        case GLFW_KEY_D:
            GLRenderer::translate_camera(STRAFE_RIGHT);
            break;

            // Rotation
        case GLFW_KEY_LEFT:
            GLRenderer::rotate_camera(YAW_LEFT);
            break;
        case GLFW_KEY_RIGHT:
            GLRenderer::rotate_camera(YAW_RIGHT);
            break;
        case GLFW_KEY_UP:
            GLRenderer::rotate_camera(PITCH_UP);
            break;
        case GLFW_KEY_DOWN:
            GLRenderer::rotate_camera(PITCH_DOWN);
            break;
        }

    GLRenderer::update_camera();

}

void GLManager::glfw_error_callback(int error, const char* description)
{
    stringstream ss;

    ss << "ERROR: Source: GLFW Code: " << error << " Message: " << description << endl;

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

        frame_count = 0;

        stringstream ss;

        ss << "Lean GL Manager - FPS: " << fps;

        glfwSetWindowTitle(window, ss.str().c_str());
    }
    else
    {
        frame_count++;
    }

    
}
