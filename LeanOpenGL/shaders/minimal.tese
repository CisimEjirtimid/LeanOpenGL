#version 450 core

layout(quads, equal_spacing , ccw) in;

out vec4 ex_Color;
void main() {
	float u = gl_TessCoord.x;
	float omu = 1 - u;
	float v = gl_TessCoord.y;
	float omv = 1 - v;
	ex_Color = vec4(1, 0, 0, 1);
	vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 b = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 p = mix(a, b, gl_TessCoord.y);
	gl_Position = p;
}


//void main()
//{
//	 float u = gl_TessCoord.x;
//	 float v = gl_TessCoord.y;
//
//	 vec4 p0 = gl_in[0].gl_Position;
//	 vec4 p1 = gl_in[1].gl_Position;
//	 vec4 p2 = gl_in[2].gl_Position;
//	 vec4 p3 = gl_in[3].gl_Position;
//
//	 vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, u);
//	 vec4 b = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, u);
//	 // Linear interpolation
//
//	 gl_Position = mix(a, b, v);
//
//	//vec4 p0 = gl_in[0].gl_Position;
//	//vec4 p1 = gl_in[1].gl_Position;
//	//vec4 p2 = gl_in[2].gl_Position;
//
//	//vec3 p = gl_TessCoord.xyz;
//
//	//gl_Position = vec4 (p0*p.x + p1*p.y + p2*p.z);
//
////gl_Position = vec4(gl_in[0].gl_Position.xyz * gl_TessCoord.x + gl_in[1].gl_Position.xyz * gl_TessCoord.y + gl_in[2].gl_Position.xyz * gl_TessCoord.z + 1.0, 1.0);
//}






//layout(triangles, equal_spacing, ccw) in;
//void main()
//{
// float u = gl_TessCoord.x;
// float v = gl_TessCoord.y;
//  float z = gl_TessCoord.z;

// vec4 p0 = gl_in[0].gl_Position;
// vec4 p1 = gl_in[1].gl_Position;
// vec4 p2 = gl_in[2].gl_Position;
// vec4 p3 = gl_in[3].gl_Position;

// vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, u);
// vec4 b = mix(gl_in[1].gl_Position, gl_in[2].gl_Position, u);
// // Linear interpolation

// gl_Position = mix(a, b, v);
//}
