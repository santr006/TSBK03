# version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 0) in vec2 UV;

uniform sampler2D framebuffer;
//uniform vec3 anArray[2];

void main(){
	//pass through position and size
    gl_Position = texture(framebuffer, UV);
	gl_PointSize = framebuffer.w;

}
