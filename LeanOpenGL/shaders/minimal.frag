#version 450

out vec4 frag_colour;

uniform int draw_wireframe;

uniform sampler2D terrain;

noperspective in vec3 dist;

in geom_data
{
    vec3 color;
    vec2 tex_coord;
} in_data;

void main()
{
    vec4 tex_color = texture(terrain, in_data.tex_coord);

    float edge_intensity = 0.0;

    if (draw_wireframe > 0)
    {
        // determine frag distance to closest edge
        float near_edge = min(min(dist[0],dist[1]),dist[2]);
        edge_intensity = exp2(-1.0*near_edge*near_edge);
    }

    frag_colour = (edge_intensity * vec4(0.1, 0.1, 0.1, 1.0)) + ((1.0 - edge_intensity) * vec4(tex_color.rgb, 1.0));
}
