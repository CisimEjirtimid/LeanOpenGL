#include "GLRenderer.h"
#include "GLCommon.h"

using namespace std;
using namespace gl;
using namespace gli;
using namespace glm;

// Declaration of static variables

// Camera statics
vec3 GLRenderer::_camera_position = vec3(5.f, 5.f, 5.f);
vec3 GLRenderer::_camera_direction = normalize(vec3(0.f, 0.f, 0.f) - _camera_position);
vec3 GLRenderer::_camera_up = vec3(0.f, 1.f, 0.f);

// PVM matrix statics
mat4 GLRenderer::_perspective_matrix = perspective(45.f, 4.f / 3.f, 0.1f, 100.f);
mat4 GLRenderer::_view_matrix = lookAt(_camera_position, _camera_position + _camera_direction , _camera_up);
mat4 GLRenderer::_model_matrix = mat4(1.f);

mat4 GLRenderer::_pvm_matrix = _perspective_matrix * _view_matrix * _model_matrix;

GLRenderer::GLRenderer()
    : _programme(glCreateProgram())
    , _objects()
{
}

GLRenderer::~GLRenderer()
{
    
}

void GLRenderer::add_object(int size_in_bytes, float* points, GLenum mode, int attribs_per_point, GLenum type, GLenum usage)
{
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size_in_bytes, points, usage);

    vector<GLuint> vbos;
    vbos.push_back(vbo);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, attribs_per_point, type, GL_FALSE, 0, nullptr);

    _objects.push_back(make_tuple(vao, mode, vbos, size_in_bytes / (sizeof(float) * attribs_per_point)));
}

void GLRenderer::add_colours(int size_in_bytes, void* colours, int attribs_per_colour, GLenum type, GLenum usage)
{
}

void GLRenderer::add_normals(int size_in_bytes, void* normals, int attribs_per_normal, GLenum type, GLenum usage)
{
}

void GLRenderer::add_tex_coords(int size_in_bytes, void* tex_coords, int attribs_per_tex_coord, GLenum type, GLenum usage)
{
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size_in_bytes, tex_coords, usage);

    VAOData& current_object = _objects.back();

    get<2>(current_object).push_back(vbo);

    GLuint& vao = get<0>(current_object);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(1, attribs_per_tex_coord, type, GL_FALSE, 0, nullptr);

    // there's no adding to _objects vector, because we are changing the state of the last added object
}

void GLRenderer::load_texture(std::string filename)
{
    texture2d ground_texture(load(filename.c_str()));

    if (ground_texture.empty())
        return;
}

void GLRenderer::load_texture(std::string filename, unsigned width, unsigned height)
{

}


void GLRenderer::load_heights(std::string filename)
{
    texture2d ground_heights(load(filename.c_str()));

    if (ground_heights.empty())
        return;
}

void GLRenderer::load_heights(std::string filename, unsigned width, unsigned height)
{
    GLuint heights;

    glGenTextures(1, &heights);

    glBindTexture(GL_TEXTURE_2D, heights);

    _textures.push_back(heights);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    short *data;
    FILE *file;

    // open texture data
    file = fopen(filename.c_str(), "rb");
    if (file == nullptr)
        return;

    // obtain file size:
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // allocate buffer
    data = static_cast<short*>(malloc(sizeof(short) * size));

    // read texture data
    long result = fread(data, sizeof(short), size, file);
    fclose(file);

    // build our texture mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_SHORT, data);

    // free buffer
    free(data);
}

void GLRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(_programme);

    // setup the textures for sampling from the shaders
    glUniform1i(uniform_heights_loc, 0); // texture unit 0 is for heights
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textures.back());

    glUniformMatrix4fv(uniform_pvm_matrix_loc, 1, GL_FALSE, value_ptr(_pvm_matrix));

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
    glLinkProgram(_programme);

    //  get uniform locations from shaders
    uniform_heights_loc = glGetUniformLocation(_programme, "heights");
    uniform_texture_loc = glGetUniformLocation(_programme, "texture");
    uniform_pvm_matrix_loc = glGetUniformLocation(_programme, "pvm_matrix");

    glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(GL_LESS);
    
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GLRenderer::translate_camera(int translate_direction)
{


    switch (translate_direction)
    {
    case MOVE_FORWARD:
        _camera_position += _camera_direction;
        break;
    case MOVE_BACKWARD:
        _camera_position -= _camera_direction;
        break;
    case STRAFE_LEFT:
        _camera_position += normalize(cross(_camera_direction, _camera_up));
        break;
    case STRAFE_RIGHT:
        _camera_position -= normalize(cross(_camera_direction, _camera_up));
        break;
    }
}

void GLRenderer::rotate_camera(int rotate_direction)
{
    float rotation_angle = 3.14f / 180.f;
    vec3 rotation_axis = cross(_camera_direction, _camera_up);

    switch (rotate_direction)
    {
    case YAW_LEFT:
        _camera_direction = rotate(_camera_direction, rotation_angle, _camera_up);
        break;
    case YAW_RIGHT:
        _camera_direction = rotate(_camera_direction, -rotation_angle, _camera_up);
        break;
    case PITCH_UP:
        _camera_direction = rotate(_camera_direction, rotation_angle, rotation_axis);
        break;
    case PITCH_DOWN:
        _camera_direction = rotate(_camera_direction, -rotation_angle, rotation_axis);
        break;
    }
}

void GLRenderer::update_camera()
{
    _view_matrix = lookAt(_camera_position, _camera_position + _camera_direction, _camera_up);
    _pvm_matrix = _perspective_matrix * _view_matrix * _model_matrix;
}
