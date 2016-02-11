# version 330

layout(location = 0) out vec4 Position;
in vec4 pos;

void main(){
	Position = pos;
}