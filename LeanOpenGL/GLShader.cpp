#include "GLShader.h"

using namespace std;
using namespace gl;

GLuint GLShader::getID()
{ return _ID; }

GLShader::GLShader(GLenum shaderType)
    : _type(shaderType)
{
    _ID = glCreateShader(shaderType);
}

GLShader::~GLShader()
{
    glDeleteShader(_ID);
}

bool GLShader::load(string fileName)
{
    ifstream fileStream(fileName);
    _code = string(istreambuf_iterator<char>(fileStream), istreambuf_iterator<char>());

    if (!_code.empty())
        return true;
    else
        return false;
}

bool GLShader::compile()
{
    if (_code.empty())
        return false;

    const char * code = _code.c_str();

    glShaderSource(_ID, 1, &code, nullptr);
    glCompileShader(_ID);

    int param;

    glGetShaderiv(_ID, GL_COMPILE_STATUS, &param);
    
    if (param == GL_TRUE)
        return true;

    else
    {
        printInfoLog();
        return false;
    }
}

string GLShader::getInfoLog() const
{
    int infologLength = 0;
    int charsWritten = 0;
    char *infoLog;
    string retStr("empty");

    glGetShaderiv(_ID, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = new char[infologLength];
        glGetShaderInfoLog(_ID, infologLength, &charsWritten, infoLog);

        retStr = string(infoLog);
        delete[] infoLog;
    }

    return retStr;
}

void GLShader::printInfoLog() const
{
    string errStr = getInfoLog();
    cerr << "ERROR: " << errStr.c_str() << endl;
}
