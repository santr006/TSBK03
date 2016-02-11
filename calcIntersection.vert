# version 330

//Find the intersection point for the photons sent through this grid cell

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 UV;

uniform mat4 MVPlightPosition;
uniform vec3 lightPos;

out vec3 vdir;
out vec2 vUV;

void main(){
		
	//create a ray for every point going from the camera to the point
	vec3 r1 = vertexPosition_modelspace - lightPos;

	gl_Position = MVPlightPosition * vec4(vertexPosition_modelspace,1);
	vdir = r1;
	vUV = UV;
}
