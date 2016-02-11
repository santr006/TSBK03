#version 330

layout(points) in;
layout(points, max_vertices=3) out;

void main()
{
	//for(int i=0; i<3; i++)
	//{
		gl_Position = gl_in[0].gl_Position;
		gl_PointSize = 5;
		EmitVertex();
	//}
	//EndPrimitive();

	//for(int i=0; i<3; i++)
	//{
		//gl_Position = gl_in[i].gl_Position + vec4(0.0, 1.0, 0.0, 0);
		//EmitVertex();
	//}
	//EndPrimitive();

}