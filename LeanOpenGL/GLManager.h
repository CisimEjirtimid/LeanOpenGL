#pragma once

// STD/STL headers
#include <vector>
#include <sstream>

// OpenGL headers
#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <gli/gli.hpp>

// Lean headers
#include "GLLogger.h"
#include "GLRenderer.h"

namespace gl
{
    //using ObjectData =  std::tuple<
    //                        std::vector<float>  /*  Vector of data  */,
    //                        long                /*  Size of vector  */
    //                    >;

    // Class for Callbacks, Object Management, Texture Management etc.

    class GLManager
    {
    private:

        std::vector<float> _patch_vertices;
        std::vector<float> _patch_tex_coords;

        GLRenderer& _renderer;
    public:
        GLManager(GLRenderer& renderer);
        virtual ~GLManager();

        // Object/Texture Management
        void create_grid(int width, int height, float step_x, float step_y);
        
        void load_texture(std::string filename);

        void load_heights(std::string filename, unsigned int width, unsigned int height);

        // Callbacks, Updates
        static void __stdcall shader_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

        static void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void glfw_window_size_callback(GLFWwindow* window, int width, int height);
        static void glfw_error_callback(int error, const char* description);
        static void update_fps_counter(GLFWwindow* window);
    };
}

