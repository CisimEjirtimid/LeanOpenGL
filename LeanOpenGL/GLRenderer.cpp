#include "GLRenderer.h"

using namespace std;
using namespace gl;

GLRenderer::GLRenderer()
    : _programme(glCreateProgram())
    , _objects()
{
}

GLRenderer::~GLRenderer()
{
    
}

void GLRenderer::addObject(int sizeInBytes, float* points, GLenum mode, int attribsPerPoint, GLenum type, GLenum usage)
{
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, points, usage);

    vector<GLuint> vbos;
    vbos.push_back(vbo);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, attribsPerPoint, type, GL_FALSE, 0, nullptr);

    _objects.push_back(make_tuple(vao, mode, vbos, sizeInBytes / (sizeof(float) * attribsPerPoint)));
}

void GLRenderer::addColours(void* colours, int attribsPerColour)
{
}

void GLRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(_programme);

    for_each(_objects.begin(), _objects.end(),
        [](VAOData& t)
    {
        glBindVertexArray(get<0>(t));

        glDrawArrays(get<1>(t), 0, get<3>(t));
    });

    glBindVertexArray(0);
}

void GLRenderer::addShader(GLenum shaderType, string fileName)
{
    GLShader shader(shaderType);

    _shaders.push_back(shader);

    shader.load(fileName);
    shader.compile();

    glAttachShader(_programme, shader.getID());
}

void GLRenderer::finishInit()
{
    // init uniforms

    glLinkProgram(_programme);

    glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(GL_LESS);
    
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
