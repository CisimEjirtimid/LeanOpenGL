#version 450 core

layout(quads, equal_spacing , ccw) in;

uniform int collision;

//uniform isampler2D heights;
uniform mat4 pvm_matrix;
uniform mat4 vm_matrix;

uniform vec2 terrain_size; // in patches
uniform vec2 patch_size;

in tesc_data
{
    vec3 color;
} in_data[];

out tese_data
{
    vec3 color; // debugging
    vec2 tex_coord;
} out_data;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, u);
    vec4 b = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, u);
    vec4 p = mix(a, b, v);

    float tex_coord_s = p.x / (terrain_size.x * patch_size.x);
    float tex_coord_t = p.z / (terrain_size.y * patch_size.y);

    out_data.tex_coord = vec2(tex_coord_s, tex_coord_t);

    //vec4 height = texture(heights, vec2(tex_coord_s, tex_coord_t));
    //p.y = height.r;

    if (collision > 0)
    {
        gl_Position = p;
    }
    else
    {
        gl_Position = pvm_matrix * p;
    }

    vec3 a_color = mix(in_data[0].color, in_data[1].color, u);
    vec3 b_color = mix(in_data[2].color, in_data[3].color, u);

    out_data.color = mix(a_color, b_color, v);

    out_data.color = vec3(tex_coord_s, tex_coord_t, 0);
}
