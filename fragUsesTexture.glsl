# version 330

layout(location = 0) out vec4 color;

uniform sampler2D sand;
uniform vec2 sizeInPixels;

void main(){
	color = vec4(1,0,0,0.5);

	//rasterize points in a high accuracy render target(fbo?)
	//z-test the points against the camera's depth buffer
	if(texture(sand, vec2(gl_FragCoord.x/sizeInPixels.x, gl_FragCoord.y/sizeInPixels.y)).x < gl_FragCoord.z) {
		color = vec4(0,0,0,0);
	}
	//z-test in geometry shader instead????

	//calculate final intensity with I = Iphoton*e^(-g*ds), g = constant for intensity fading in the medium, ds = distance from surface
	
}
