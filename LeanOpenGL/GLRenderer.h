#pragma once

// STD/STL headers
#include <vector>
#include <tuple>
#include <algorithm>

// OpenGL headers
#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
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
        GLuint _programme;
        std::vector<GLShader> _shaders;
        std::vector<VAOData> _objects;
        std::vector<GLuint> _textures;

        gli::texture _ground_texture, _ground_heights;

        GLint uniform_heights_loc;
        GLint uniform_texture_loc;
        GLint uniform_pvm_matrix_loc;

        static glm::mat4 _perspective_matrix;
        static glm::mat4 _view_matrix;
        static glm::mat4 _model_matrix;

        static glm::mat4 _pvm_matrix;

        static glm::vec3 _camera_position;
        static glm::vec3 _camera_direction;
        static glm::vec3 _camera_up;

        static glm::vec3 _camera_pitch_axis;
        static glm::vec3 _camera_yaw_axis;

    public:
        GLRenderer();
        ~GLRenderer();


        /*
            Function add_object intializes adding of new object, 
            then the other add functions (colours, normals, tex_coords)
            are working with most recently added vertices
            (change the state most recently object)

            current vertex variable layout:
            location 0 = vertex_position
            location 1 = tex_coords

            TO DO:
            set vertex variable layout to reperesent the order of the function calling
        */
        void add_object(int size_in_bytes, float* points, GLenum mode, int attribs_per_point = 3, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        void add_colours(int size_in_bytes, void* colours, int attribs_per_colour = 3, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        void add_normals(int size_in_bytes, void* normals, int attribs_per_normal = 3, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        void add_tex_coords(int size_in_bytes, void* tex_coords, int attribs_per_tex_coord = 2, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);

        void load_texture(std::string filename);
        void load_texture(std::string filename, unsigned int width, unsigned int height);

        void load_heights(std::string filename);
        void load_heights(std::string filename, unsigned int width, unsigned int height);

        void draw();

        void addShader(GLenum shaderType, std::string fileName);

        void finishInit();

        static void translate_camera(int translate_direction);
        static void rotate_camera(int rotate_direction);
        static void update_camera();
    };
}
