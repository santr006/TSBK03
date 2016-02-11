# version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 UV;

out vec4 estimatedPoint;

uniform sampler2D estimatedPointOnGeometry;
uniform mat4 MVPcameraPosition;
uniform vec3 cameraPos;
uniform float smax;
uniform float smin;
uniform float znear;
uniform float zfar;

void main(){

	//transform the intersection points to camera-space
	estimatedPoint = texture(estimatedPointOnGeometry, UV);
	vec4 p = MVPcameraPosition * estimatedPoint;

	//Point closer to the camera should be bigger on the screen
	//calculate each point's size with s = a + b/dc, dc = distance from camera,
	float a = smax - zfar*(smax-smin)/(zfar-znear); 
	float b = znear*zfar*(smax-smin)/(zfar-znear);
	float dc = length(cameraPos - vec3(p.x,p.y,p.z));
	
	float s = a + b/dc;
	estimatedPoint.w = s;
}
