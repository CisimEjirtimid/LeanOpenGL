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

        std::vector<float> patch_vector;
        gli::texture _ground_texture, _ground_heights;

        GLRenderer& _renderer;
    public:
        GLManager(GLRenderer& renderer);
        virtual ~GLManager();

        // Object/Texture Management
        void createGrid(int width, int height, float step_x, float step_y);
        void loadTexture(std::string filename);
        void loadHeights(std::string filename);

        // Callbacks, Updates
        static void glfw_error_callback(int error, const char* description);
        static void update_fps_counter(GLFWwindow* window);
    };
}

