#include "GLManager.h"

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
    texture ground_texture = load(filename.c_str());

    if (ground_texture.empty())
        return;
}

void GLManager::loadHeights(std::string filename)
{
    texture ground_heights = load(filename.c_str());

    if (ground_heights.empty())
        return;
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
