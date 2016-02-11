# version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

out vec3 normal;

uniform mat4 MVPlightPosition;


void main(){
	//subdivide the grid cells to get the desired number of points
	//skip

	//send them on
    gl_Position =  MVPlightPosition * vec4(vertexPosition_modelspace,1);
	normal = vertexNormal_modelspace;
}
