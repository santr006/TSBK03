# version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

//out vec4 thePoint;
//out mat4 MVPlight;
out vec2 UV;

uniform mat4 MVP;
//uniform sampler2D estimatedPoint;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	UV = vertexUV;
	//thePoint = MVP * texture(estimatedPoint, vertexUV);
	//MVPlight = MVP;
}
