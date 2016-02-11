# version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;

out vec4 pos;
out vec3 norm;

uniform mat4 MVP;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	pos = vec4(vertexPosition_modelspace,1);
	norm = vertexNormal;
}
