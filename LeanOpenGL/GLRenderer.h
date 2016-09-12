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

        GLuint tbo;
        GLuint query;

        // samplers
        GLint _uniform_heights_loc;
        GLint _uniform_terrain_loc;
        
        // mat4
        GLint _uniform_pvm_matrix_loc;
        GLint _uniform_vm_matrix_loc;

        // vec2
        GLint _uniform_terrain_size_loc;
        GLint _uniform_patch_size_loc;

        // tess control
        GLint _uniform_tesc_outer_loc;
        GLint _uniform_tesc_inner_loc;
        
        // wireframe uniforms
        GLint _uniform_win_scale_loc;
        GLint _uniform_draw_wireframe_loc;
        GLint _uniform_wireframe_color_loc;

        // collision detection
        GLint _uniform_camera_position_loc;
        GLint _uniform_collision_loc;

        static glm::mat4 _perspective_matrix;
        static glm::mat4 _view_matrix;
        static glm::mat4 _model_matrix;

        static glm::mat4 _pvm_matrix;

        static glm::vec3 _camera_position;
        static glm::vec3 _camera_direction;
        static glm::vec3 _camera_up;

        static glm::vec3 _camera_pitch_axis;
        static glm::vec3 _camera_yaw_axis;

        static float _tesc_outer;
        static float _tesc_inner;

        static glm::vec2 _window_size;
        static bool _wireframe;

        static glm::vec3 _wireframe_color;

        float* _feedback;

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
                - set vertex variable layout to reperesent the order of the function calling
                - implement add_colours & add_normals functions
        */
        void add_object(int size_in_bytes, float* points, GLenum mode, int attribs_per_point = 3, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        void add_colours(int size_in_bytes, void* colours, int attribs_per_colour = 3, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        void add_normals(int size_in_bytes, void* normals, int attribs_per_normal = 3, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);
        void add_tex_coords(int size_in_bytes, void* tex_coords, int attribs_per_tex_coord = 2, GLenum type = GL_FLOAT, GLenum usage = GL_STATIC_DRAW);

        void enable_transform_feedback();

        void load_texture(std::string filename);
        void load_heights(std::string filename, unsigned int width, unsigned int height);
        
        
        
        void draw();

        void add_shader(GLenum shaderType, std::string fileName);

        void finish_init();

        static void invert_wireframe();
        static void translate_camera(int translate_direction);
        static void rotate_camera(int rotate_direction);
        static void update_camera();

        static void set_window_size(GLFWwindow* window);

        static void increase_tesc_outer();
        static void increase_tesc_inner();

        static void decrease_tesc_outer();
        static void decrease_tesc_inner();
    };
}
