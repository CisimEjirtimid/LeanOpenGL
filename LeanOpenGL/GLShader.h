#pragma once

// STD/STL headers
#include <streambuf>
#include <iostream>
#include <fstream>

// OpenGL headers
#include <GL/glew.h>

// Lean headers

namespace gl
{
    class GLShader
    {
    public:
        GLShader(GLenum shaderType);
        virtual ~GLShader();

        bool    load(std::string fileName);
        bool    compile();

        GLuint getID();

        std::string getInfoLog() const;
        void printInfoLog() const;

    protected:
        GLenum  _type; // GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER
        GLuint  _ID;
        std::string   _code;
    };
}
