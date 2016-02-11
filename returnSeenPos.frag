# version 330

in mat4 MVPlightPositionInv;
in vec3 normal;

layout(location = 1) out vec4 seenPos;
layout(location = 2) out vec3 seenNormal;

void main(){
	//intersect the rays with the water surface

	//transform the intersection points to world-space
	seenPos = MVPlightPositionInv * gl_FragCoord;
	seenNormal = normal;
}