# version 330

layout(location = 0) out float fragmentDepth;
//in float hej;

void main(){
	fragmentDepth = gl_FragCoord.z;//hej;vec3(1,0,0);
}