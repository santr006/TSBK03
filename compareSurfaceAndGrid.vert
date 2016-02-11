# version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

out vec3 normals;

uniform mat4 MVP;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	vec4 normals4 = MVP * vec4(vertexNormal_modelspace,1);
	normals = vec3(normals4.x, normals4.y, normals4.z);
}
