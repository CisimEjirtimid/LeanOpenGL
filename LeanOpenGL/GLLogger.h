#pragma once

// STD/STL headers
#include <time.h>
#include <iostream>
#include <fstream>

// OpenGL headers

// Lean headers

#define GL_LOG_FILE "leanGL.log"

namespace gl
{
    class GLLogger
    {
    private:
        std::fstream fs;

    public:
        GLLogger();
        ~GLLogger();

        bool time_log();
        bool error_log(const char* message);

    };
}

