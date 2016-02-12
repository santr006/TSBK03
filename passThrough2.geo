#version 330

layout(points) in;
layout(points, max_vertices=2) out;

in vec2 vUV[];
out vec2 fUV;

in vec4 vtex[];
out vec4 ftex;

//extra
in vec4 estimatedPoint[];
out vec4 fEstimatedPoint;
in vec3 refRay[];
out vec3 fRefRay;

void main()
{
	fUV = vUV[0];
	ftex = vtex[0];
	
	/*gl_Position = ftex;//gl_in[0].gl_Position;
	gl_PointSize = 1;
	EmitVertex();
	EndPrimitive();*/

	gl_Position = estimatedPoint[0];//ftex;//gl_in[0].gl_Position;
	gl_PointSize = 5;
	fEstimatedPoint = estimatedPoint[0];
	EmitVertex();
	EndPrimitive();

	//extra
	
	fRefRay = refRay[0];
}