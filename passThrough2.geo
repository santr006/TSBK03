#version 330

layout(points) in;
layout(points, max_vertices=1) out;

//in vec2 vUV[];
//out vec2 fUV;

in vec4 vtex[];
out vec4 ftex;

void main()
{
	ftex = vtex[0];

	gl_Position = ftex;//gl_in[0].gl_Position;
	gl_PointSize = 1;
	EmitVertex();

}