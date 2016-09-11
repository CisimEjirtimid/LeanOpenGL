#version 450

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_position;

uniform isampler2D heights;

uniform vec2 terrain_size; // in patches
uniform vec2 patch_size;

out vertex_data
{
    vec3 color;
} out_data;

void main()
{
    float tex_coord_s = vertex_position.x / (terrain_size.x * patch_size.x);
    float tex_coord_t = vertex_position.y / (terrain_size.y * patch_size.y);

    vec4 height = texture(heights, vec2(tex_coord_s, tex_coord_t));

    gl_Position = vec4(vertex_position.x, height.r, vertex_position.y, 1.0);
}
