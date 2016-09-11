#include "GLRenderer.h"
#include "GLCommon.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/raw_data.hpp>
#include <gli/gli.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace std;
using namespace gl;
using namespace glm;

// Declaration of static variables

// Camera statics
vec3 GLRenderer::_camera_position = vec3(0.f, 1000.f, 0.f);
vec3 GLRenderer::_camera_direction = normalize(vec3(1000.f, 0.f, 1000.f) - _camera_position);
vec3 GLRenderer::_camera_up = vec3(0.f, 1.f, 0.f);

// PVM matrix statics
mat4 GLRenderer::_perspective_matrix = perspective(45.f, 4.f / 3.f, 0.1f, 1000000.f);
mat4 GLRenderer::_view_matrix = lookAt(_camera_position, _camera_position + _camera_direction , _camera_up);
mat4 GLRenderer::_model_matrix = mat4(1.f);

mat4 GLRenderer::_pvm_matrix = _perspective_matrix * _view_matrix * _model_matrix;

// Tesc parameter statics
float GLRenderer::_tesc_outer = 2.0;
float GLRenderer::_tesc_inner = 2.0;

// Wireframe control static
bool GLRenderer::_wireframe = false;
vec3 GLRenderer::_wireframe_color = vec3(0.0, 0.0, 0.0);

GLRenderer::GLRenderer()
    : _programme(glCreateProgram())
    , _objects()
{
    _feedback = new GLfloat[12];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

GLRenderer::~GLRenderer()
{
    delete _feedback;
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

void GLRenderer::enable_transform_feedback()
{
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * 4, nullptr, GL_STATIC_READ); // sizeof

    glGenQueries(1, &query);
}

void GLRenderer::load_texture(std::string filename)
{
    GLuint texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    _textures.push_back(texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int x, y, comp;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &comp, STBI_rgb);

    // build our texture mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // free buffer
    stbi_image_free(data);
}

void GLRenderer::load_heights(std::string filename, unsigned width, unsigned height)
{
    GLuint heights;

    glGenTextures(1, &heights);

    glBindTexture(GL_TEXTURE_2D, heights);

    _textures.push_back(heights);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    byte *data;
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
    data = static_cast<byte*>(malloc(sizeof(byte) * size));

    // read texture data
    long result = fread(data, sizeof(byte), size, file);
    fclose(file);

    // build our texture mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16I, width, height, 0, GL_RED_INTEGER, GL_SHORT, data);

    // free buffer
    free(data);
}

void GLRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(_programme);

    #pragma region Uniform Setup
    // setup the textures for sampling from the shaders
    glUniform1i(_uniform_heights_loc, 0); // texture unit 0 is for heights
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textures[1]);

    glUniform1i(_uniform_terrain_loc, 2); // texture unit 2 is for terrain
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _textures[0]);

    // pvm matrix uniform
    glUniformMatrix4fv(_uniform_pvm_matrix_loc, 1, GL_FALSE, value_ptr(_pvm_matrix));
    glUniformMatrix4fv(_uniform_vm_matrix_loc, 1, GL_FALSE, value_ptr(_view_matrix * _model_matrix));
    
    // patch and terrain size uniforms
    glUniform2f(_uniform_terrain_size_loc, 1200.f, 1200.f);
    glUniform2f(_uniform_patch_size_loc, 70.f, 90.f);

    // tesselation parameters uniforms
    glUniform1f(_uniform_tesc_outer_loc, _tesc_outer);
    glUniform1f(_uniform_tesc_inner_loc, _tesc_inner);

    // window scale uniform
    glUniform2fv(_uniform_win_scale_loc, 1, value_ptr(_window_size));
    
    // wireframe uniform
    glUniform1i(_uniform_draw_wireframe_loc, _wireframe);
    glUniform3fv(_uniform_wireframe_color_loc, 1, value_ptr(_wireframe_color));

    // camera position uniform
    glUniform3fv(_uniform_camera_position_loc, 1, value_ptr(_camera_position));
    #pragma endregion


    #pragma region Collision Detection (Transform Feedback)
    glUniform1i(_uniform_collision_loc, 1);
    glEnable(GL_RASTERIZER_DISCARD);
    
    // TODO: parametrize call of transform feedback (different tbo for every object)
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    glBeginTransformFeedback(GL_TRIANGLES);

    for_each(_objects.begin(), _objects.end(),
        [](VAOData& t)
    {
        glBindVertexArray(get<0>(t));

        glDrawArrays(get<1>(t), 0, get<3>(t));
    });

    glBindVertexArray(0);

    glEndTransformFeedback();
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

    glFlush();

    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 3 * sizeof(glm::vec4), _feedback);

    GLuint primitives;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

    cout << primitives << " primitives written." << endl << endl;

    if (primitives > 0)
    {
        float t = _feedback[0];
        float tc = _feedback[1];
        float s = _feedback[2];

        float diff = _feedback[4];

        cout << t << " " << tc << " " << s << " " << diff << endl << endl;
        cout << " Camera colided with terrain!" << endl << endl;

        vec3 hit = normalize(_camera_position) * t;

        cout << hit.x << " " << hit.y << " " << hit.z << endl << endl;
        cout << _camera_position.x << " " << _camera_position.y << " " << _camera_position.z << endl << endl;

        _camera_position.y *= 1.1f;

        update_camera();
    }
    glDisable(GL_RASTERIZER_DISCARD);
    #pragma endregion


    glUniform1i(_uniform_collision_loc, 0);

    for_each(_objects.begin(), _objects.end(),
        [](VAOData& t)
    {
        glBindVertexArray(get<0>(t));

        glDrawArrays(get<1>(t), 0, get<3>(t));
    });
}

void GLRenderer::add_shader(GLenum shaderType, string fileName)
{
    GLShader shader(shaderType);

    _shaders.push_back(shader);

    shader.load(fileName);
    shader.compile();

    GLint isCompiled = 0;

    glGetShaderiv(shader.getID(), GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        std::cout << "Error compiling shader " << fileName << "!" << std::endl;
        GLint maxLength = 0;
        glGetShaderiv(shader.getID(), GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader.getID(), maxLength, &maxLength, &errorLog[0]);

        std::string error(errorLog.begin(), errorLog.end());
        std::cout << error << std::endl;

        glDeleteShader(shader.getID());
        return;
    }

    glAttachShader(_programme, shader.getID());
}

void GLRenderer::finish_init()
{
    const GLchar* _feedback_varyings[] = { "gl_Position" };
    glTransformFeedbackVaryings(_programme, 1, _feedback_varyings, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(_programme);

    GLint isLinked = 0;

    glGetProgramiv(_programme, GL_LINK_STATUS, &isLinked);
    
    if (isLinked == GL_FALSE)
    {
        std::cout << "Error linking program!" << std::endl;
        GLint maxLength = 0;
        glGetProgramiv(_programme, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(_programme, maxLength, &maxLength, &errorLog[0]);

        std::string error(errorLog.begin(), errorLog.end());
        std::cout << error << std::endl;

        glDeleteProgram(_programme);
        return;
    }

    #pragma region Shader Uniform Locations
    _uniform_heights_loc = glGetUniformLocation(_programme, "heights");
    _uniform_terrain_loc = glGetUniformLocation(_programme, "terrain");

    _uniform_pvm_matrix_loc = glGetUniformLocation(_programme, "pvm_matrix");
    _uniform_vm_matrix_loc = glGetUniformLocation(_programme, "vm_matrix");

    _uniform_terrain_size_loc = glGetUniformLocation(_programme, "terrain_size");
    _uniform_patch_size_loc = glGetUniformLocation(_programme, "patch_size");

    _uniform_tesc_outer_loc = glGetUniformLocation(_programme, "tesc_outer");
    _uniform_tesc_inner_loc = glGetUniformLocation(_programme, "tesc_inner");

    _uniform_win_scale_loc = glGetUniformLocation(_programme, "win_scale");
    _uniform_draw_wireframe_loc = glGetUniformLocation(_programme, "draw_wireframe");
    _uniform_wireframe_color_loc = glGetUniformLocation(_programme, "wireframe_color");

    _uniform_camera_position_loc = glGetUniformLocation(_programme, "camera_position");
    _uniform_collision_loc = glGetUniformLocation(_programme, "collision");
    #pragma endregion

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glClearColor(0.2f, 0.7f, 1.0f, 1.0f);
}

void GLRenderer::set_window_size(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    _window_size = vec2(static_cast<float>(width), static_cast<float>(height));
}

void GLRenderer::invert_wireframe()
{
    if (!_wireframe)
    {
        float r = (float)(rand() % 256) / 256.f;
        float g = (float)(rand() % 256) / 256.f;
        float b = (float)(rand() % 256) / 256.f;

        _wireframe_color = vec3(r, g, b);
    }

    _wireframe = !_wireframe;
}

void GLRenderer::translate_camera(int translate_direction)
{
    float speed = 100.f;

    switch (translate_direction)
    {
    case MOVE_FORWARD:
        _camera_position += speed * _camera_direction;
        break;
    case MOVE_BACKWARD:
        _camera_position -= speed * _camera_direction;
        break;
    case STRAFE_LEFT:
        _camera_position -= speed * normalize(cross(_camera_direction, _camera_up));
        break;
    case STRAFE_RIGHT:
        _camera_position += speed * normalize(cross(_camera_direction, _camera_up));
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

void GLRenderer::increase_tesc_outer()
{
    _tesc_outer += 0.5f;
}

void GLRenderer::increase_tesc_inner()
{
    _tesc_inner += 0.5f;
}

void GLRenderer::decrease_tesc_outer()
{
    _tesc_outer -= 0.5f;
}

void GLRenderer::decrease_tesc_inner()
{
    _tesc_inner -= 0.5f;
}
