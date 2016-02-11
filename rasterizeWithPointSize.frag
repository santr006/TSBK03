# version 330

in mat4 camPos;

layout(location = 0) out vec4 color;

uniform sampler2D depthbuffer;

void main(){
	//rasterize points in a high accuracy render target(fbo?)
	//z-test the points against the camera's depth buffer

	//calculate final intensity with I = Iphoton*e^(-g*ds), g = konstant for intensity fading in the medium, ds = distance from surface
}