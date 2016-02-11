# version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 UV;

uniform sampler2D estimatedPointOnGeometry;//in world-space
uniform sampler2D intersectionPointOnWater;//in world-space
uniform sampler2D refractedRay;
uniform mat4 MVPlightPosition;

void main(){
	
	vec3 r2 = texture(refractedRay, UV).xyz;
	vec3 v = texture(intersectionPointOnWater, UV).xyz;

	//use the distance from the old point to the intersection point to find the new d
	float d = length(texture(estimatedPointOnGeometry, UV).xyz - v);

	vec3 p = v + d * r2;

	//project p into light-space and find its texture coordinates for depth if it would have been on the geometry
	//send to fragment shader
	gl_Position = MVPlightPosition * vec4(v,1);
}
