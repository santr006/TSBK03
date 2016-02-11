# version 330

layout(points) in;
layout(points, max_vertices=1) out;

in vec3 vdir[];
out vec3 fdir;

in vec2 vUV[];
out vec2 fUV;

void main(){
	gl_Position = gl_in[0].gl_Position;
	gl_PointSize = 10;
	EmitVertex();
	
	fdir = vdir[0];
	fUV = vUV[0];
}
