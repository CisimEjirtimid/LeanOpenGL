#version 450
#extension GL_NV_gpu_shader5 : enable

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_position;

uniform sampler2D heights;
uniform mat4 pvm_matrix;

void main()
{
    //int16_t height = texture2D(heights, texture_position);

    vec4 height = texture2D(heights, texture_position);

    vec4 temp_pos = vec4(vertex_position.x, height.x, vertex_position.y, 1.0);

    gl_Position = pvm_matrix * temp_pos;
}
