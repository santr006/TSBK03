# version 330

//Transforms the poisitions in the framebuffer from light space to world space
//not canonial screen space at the moment

layout(location = 0) in vec3 vertexPosition_modelspace;

out mat4 camPos;

uniform mat4 MVPcameraPosition;
uniform sampler2D framebuffer;
//uniform vec3 anArray[2];

void main(){
	//transform the intersection points to camera-space
    gl_Position =  MVPcameraPosition * framebuffer;

	camPos = MVPcameraPosition;
}
