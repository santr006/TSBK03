#version 330

layout(points) in;
layout(points, max_vertices=1) out;

in vec2 vUV[];
out vec2 fUV;

in vec4 vtex[];
out vec4 ftex;

//extra
in vec4 estimatedPoint[];
out vec4 fEstimatedPoint;

void main()
{
	ftex = vtex[0];

	gl_Position = gl_in[0].gl_Position;//estimatedPoint[0];//ftex;//gl_in[0].gl_Position;
	gl_PointSize = 1;
	EmitVertex();

	//extra
	fEstimatedPoint = estimatedPoint[0];
}