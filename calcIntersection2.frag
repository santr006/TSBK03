# version 330

layout(location = 0) out vec4 estimatedPointInWorldSpace;

in mat4 MVPlightPositionInv;

void main(){

	estimatedPointInWorldSpace = vec4(1,0,0,0);//MVPlightPositionInv * gl_FragCoord;

}