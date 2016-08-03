#pragma once

// STD/STL headers
#include <vector>
#include <tuple>
#include <algorithm>

// OpenGL headers
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Lean headers
#include "GLShader.h"
#include "GLRenderer.h"

namespace gl
{
    //using ObjectData =  std::tuple<
    //                        std::vector<float>  /*  Vector of data  */,
    //                        long                /*  Size of vector  */
    //                    >;

    class GLManager
    {
    private:
        std::vector<float> patch_vector;

        GLRenderer& _renderer;
    public:
        GLManager(GLRenderer& renderer);
        virtual ~GLManager();

        void createGrid(int width, int height, float step_x, float step_y);
    };
}

