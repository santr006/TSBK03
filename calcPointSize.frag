# version 330

layout(location = 3) out vec4 estimatedPointInWorldSpace;

in mat4 MVPlightPositionInv;
in vec4 estimatedPoint;

void main(){
//save size s
	estimatedPointInWorldSpace = MVPlightPositionInv * gl_FragCoord;
	estimatedPointInWorldSpace.w = s;

}