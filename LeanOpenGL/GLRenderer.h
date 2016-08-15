#pragma once

// STD/STL headers
#include <vector>
#include <tuple>
#include <algorithm>

// OpenGL headers
#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <gli/gli.hpp>

// Lean headers
#include "GLShader.h"

namespace gl
{
    using VAOData = std::tuple<
        GLuint              /* VAO           */,
        GLenum              /* Mode          */,
        std::vector<GLuint> /* VBOs          */,
        long                /* # Of Elements */
    >;

    class GLRenderer
    {
    protected:
        GLuint  _programme;
        std::vector<GLShader> _shaders;

        std::vector<VAOData> _objects;

    public:
        GLRenderer();
        ~GLRenderer();

        void addObject(int sizeInBytes, float* points, GLenum mode, int attribsPerPoint = 3, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);

        void addColours(void* colours, int attribsPerColour = 3);

        void addNormals(void* normals, int attribsPerNormal = 3);

        void addTexCoords(void* texCoords, int attribsPerTexCoord = 2);

        void draw();

        void addShader(GLenum shaderType, std::string fileName);

        void finishInit();
    };
}
