#version 450 core

layout (vertices = 4) out;

uniform float tesc_outer;
uniform float tesc_inner;

// debug
//in vertex_data
//{
//    vec3 color;
//} in_data[];

//out tesc_data
//{
//    vec3 color;
//} out_data[];


void main()
{
    // debug
    //vec3 temp_color[4];
    //temp_color[0] = vec3(1.0, 0.0, 0.0);
    //temp_color[1] = vec3(0.0, 1.0, 0.0);
    //temp_color[2] = vec3(0.0, 0.0, 1.0);
    //temp_color[3] = vec3(0.5, 0.0, 0.5);
    //out_data[gl_InvocationID].color = temp_color[gl_InvocationID % 4];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = tesc_outer;
    gl_TessLevelOuter[1] = tesc_outer;
    gl_TessLevelOuter[2] = tesc_outer;
    gl_TessLevelOuter[3] = tesc_outer;

    gl_TessLevelInner[0] = tesc_inner;
    gl_TessLevelInner[1] = tesc_inner;

}
