# version 330

layout(location = 0) out vec4 color;

in vec2 UV;

uniform sampler2D buffer1;
uniform sampler2D buffer2;

void main(){
	color = texture(buffer1, UV) + texture(buffer2, UV);
}