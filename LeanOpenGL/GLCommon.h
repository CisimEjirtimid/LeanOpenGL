#pragma once

namespace gl
{
    static const char* source_str[] =
    {
        "Source: API ",
        "Source: WINDOW SYSTEM ",
        "Source: SHADER COMPILER ",
        "Source: THIRD PARTY ",
        "Source: APPLICATION ",
        "Source: OTHER "
    };

    static const char* type_str[] =
    {
        "Type: ERROR ",
        "Type: DEPRECATED BEHAVIOUR ",
        "Type: UNDEFINED BEHAVIOUR ",
        "Type: PORTABILITY ",
        "Type: PERFORMANCE ",
        "Type: OTHER "
    };

    static const char* severity_str[] =
    {
        "Severity: HIGH ",
        "Severity: MEDIUM ",
        "Severity: LOW ",
        "Severity: NOTIFICATION"
    };

    enum translation_type
    {
        MOVE_FORWARD,
        MOVE_BACKWARD,
        STRAFE_LEFT,
        STRAFE_RIGHT
    };

    enum rotation_type
    {
        YAW_LEFT,
        YAW_RIGHT,
        PITCH_UP,
        PITCH_DOWN
    };
}
