# version 330

layout(location = 0) out vec4 pos;
//in float hej;

//in vec4 thePoint;
in vec2 UV;

uniform sampler2D estimatedPoint;

void main(){
	pos = vec4(1,0,0,0);//texture(estimatedPoint, UV);//thePoint;
}