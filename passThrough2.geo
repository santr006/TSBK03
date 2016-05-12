#version 330

layout(points) in;
layout(points, max_vertices=1) out;

in vec4 estimatedPoint[];
in float size[];

void main()
{
	gl_Position = estimatedPoint[0];//gl_in[0].gl_Position;
	gl_PointSize = size[0];

	EmitVertex();
	EndPrimitive();
}