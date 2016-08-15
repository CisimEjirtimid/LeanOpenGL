#pragma once


#include <GL/glew.h>
#include <vector>

namespace gl
{
    const char* source_str[] =
    {
        "Source: API ",
        "Source: WINDOW SYSTEM ",
        "Source: SHADER COMPILER ",
        "Source: THIRD PARTY ",
        "Source: APPLICATION ",
        "Source: OTHER "
    };

    const char* type_str[] =
    {
        "Type: ERROR ",
        "Type: DEPRECATED BEHAVIOUR ",
        "Type: UNDEFINED BEHAVIOUR ",
        "Type: PORTABILITY ",
        "Type: PERFORMANCE ",
        "Type: OTHER "
    };

    const char* severity_str[] =
    {
        "Severity: HIGH ",
        "Severity: MEDIUM ",
        "Severity: LOW ",
        "Severity: NOTIFICATION"
    };
}
