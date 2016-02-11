#version 330

layout(points) in;
layout(points, max_vertices=3) out;

in vec4 thePoint[];
out vec4 point;

//uniform mat4 MVP[];

void main()
{
	gl_Position = gl_in[0].gl_Position;
	gl_PointSize = 1;
	EmitVertex();
	//EndPrimitive();

	//for(int i = 0; i < abs(thePoint[0].x); i++)
	//{
	//gl_Position = thePoint[0];//vec4(0,0,0,1);
	//gl_PointSize = 1;
	//EmitVertex();
	//}

	point = thePoint[0];
}