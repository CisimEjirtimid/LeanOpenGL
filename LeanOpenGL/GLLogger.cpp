#include "GLLogger.h"

using namespace std;
using namespace gl;

GLLogger::GLLogger()
{
    fs.open(GL_LOG_FILE, ios_base::out | ios_base::app);
}


GLLogger::~GLLogger()
{
    if (fs.is_open())
        fs.close();
}

bool GLLogger::time_log()
{
    if (fs.is_open())
    {
        time_t current = time(nullptr);
        fs << asctime(localtime(&current)) << endl;
        return true;
    }
    else
    {
        cerr << "ERROR: Could not open file" << endl;
        return false;
    }
}

bool GLLogger::error_log(const char* message)
{
    if (fs.is_open())
    {
        fs << message << endl;
        cerr << message << endl;
        return true;
    }
    else
    {
        cerr << "ERROR: Could not open file" << endl;
        return false;
    }
}

