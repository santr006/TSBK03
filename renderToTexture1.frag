# version 330

layout(location = 0) out vec4 waterPoint;
layout(location = 2) out vec3 waterPointNormal;

in vec4 pos;
in vec3 norm;

void main(){
	waterPoint = pos;
	waterPointNormal = norm;
}