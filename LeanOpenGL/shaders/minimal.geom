#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform vec2 win_scale;

uniform vec3 camera_position;

uniform int collision;

in tese_data
{
    vec3 color;
    vec2 tex_coord;
} in_data[];

out geom_data
{
    vec3 color;
    vec2 tex_coord;
} out_data;

noperspective out vec3 dist;

int triangle_intersection(in vec3   V1,  // Triangle vertices
                            in vec3   V2,
                            in vec3   V3,
                            in vec3    O,  //Ray origin
                            in vec3    D,  //Ray direction
                            out float intersection)
{
    float EPSILON = 0.000001;

    vec3 e1, e2;  //Edge1, Edge2
    vec3 P, Q, T;
  
    float det, inv_det, u, v;
    float t;

    //Find vectors for two edges sharing V1
    e1 = V2 - V1;
    e2 = V3 - V1;

    //Begin calculating determinant - also used to calculate u parameter
    P = cross(D, e2);

    //if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
    det = dot(e1, P);

    //NOT CULLING
    if(det > -EPSILON && det < EPSILON)
        return 0;
  
    inv_det = 1.f / det;

    //calculate distance from V1 to ray origin
    T = O - V1;

    //Calculate u parameter and test bound
    u = dot(T, P) * inv_det;

    //The intersection lies outside of the triangle
    if(u < 0.f || u > 1.f)
        return 0;

    //Prepare to test v parameter
    Q = cross(T, e1);

    //Calculate V parameter and test bound
    v = dot(D, Q) * inv_det;

    //The intersection lies outside of the triangle
    if(v < 0.f || u + v > 1.f)
        return 0;

    t = dot(e2, Q) * inv_det;

    if(t > EPSILON) //ray intersection
    {
       intersection = t;
       return 1;
    }

    // No hit, no win
    return 0;
}

void main(void)
{
    if (collision > 0)
    {
        vec3 camera_direction = normalize(camera_position);
        vec3 origin = vec3(0.0);

        vec3 normal = cross(gl_in[1].gl_Position.xyz-gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz);

        float t;
        float tc = length(camera_position);
        float s = dot(camera_direction, normal);

        int res = triangle_intersection(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, origin, camera_direction, t);

        vec3 hit = camera_direction * t;

        float diff = abs(length(camera_position - hit));

        if (res == 0 || diff > 3000)
            return;

        if ((tc < t && s > 0) || (tc > t && s < 0)) // beneath the terrain
        {
            gl_Position = vec4(t, tc, s, 1);
            EmitVertex();

            gl_Position = vec4(diff, 0, 0, 1);
            EmitVertex();

            gl_Position = vec4(0, 0, 0, 1);
            EmitVertex();
            EndPrimitive();
        }
    }
    else
    {
        // taken from 'Single-Pass Wireframe Rendering'
        vec2 p0 = win_scale * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
        vec2 p1 = win_scale * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;
        vec2 p2 = win_scale * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;

        vec2 v0 = p2-p1;
        vec2 v1 = p2-p0;
        vec2 v2 = p1-p0;
    
        float area = abs(v1.x*v2.y - v1.y * v2.x);

        vec3 distv[3];
        distv[0] = vec3(area/length(v0),0,0);
        distv[1] = vec3(0,area/length(v1),0);
        distv[2] = vec3(0,0,area/length(v2));

        int i;

        for (i = 0; i < gl_in.length(); i++)
        {
            dist = distv[i];

            gl_Position = gl_in[i].gl_Position;

            out_data.color = in_data[i].color;
            out_data.tex_coord = in_data[i].tex_coord;

            EmitVertex();
        }
        EndPrimitive();
    }
}