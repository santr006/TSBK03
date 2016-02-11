# version 330

in mat4 camPos;

layout(location = 0) out vec4 color;

void main(){
	//color = gl_FragCoord;

	//Point closer to the camera should be bigger on the screen
	//calculate each point's size with s = a + b/dc, dc = distance from camera,
	// a = smax - zfar*(smax-smin)/(zfar-znear), b = znear*zfar*(smax-smin)/(zfar-znear)

	float smax = 20;
	float smin = 5;
	float znear = 1;
	float zfar = 10;
	float = smax - zfar*(smax-smin)/(zfar-znear);
	float b = znear*zfar*(smax-smin)/(zfar-znear);

	vec4 dcVec4 = camPos * vec4(0,0,0,0) - gl_FragCoord;
	float dc = sqrt(dcVec4.x*dcVec4.x + dcVec4.y*dcVec4.y + dcVec4.z*dcVec4.z);

	float size = a + b/dc;

	color = vec4(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, size);

	//rasterize points in a high accuracy render target(fbo?)
	//z-test the points against the camera's depth buffer

	//calculate final intensity with I = Iphoton*e^(-g*ds), g = konstant for intensity fading in the medium, ds = distance from surface
}