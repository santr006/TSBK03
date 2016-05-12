# version 330

layout(location = 0) out vec4 color;

in vec2 UV;

uniform sampler2D buffer1; //caustics
uniform sampler2D buffer2; //water
uniform sampler2D buffer3; //sand

void main(){

	color = vec4(0,0,0,0);

	//5*5 filter
	color += texture(buffer1, UV + vec2(-2/700, 2/700));
	color += texture(buffer1, UV + vec2(-1/700, 2/700));
	color += texture(buffer1, UV + vec2(0, 2/700));
	color += texture(buffer1, UV + vec2(1/700, 2/700));
	color += texture(buffer1, UV + vec2(2/700, 2/700));

	color += texture(buffer1, UV + vec2(-2/700, 1/700));
	color += texture(buffer1, UV + vec2(-1/700, 1/700));
	color += texture(buffer1, UV + vec2(0, 1/700));
	color += texture(buffer1, UV + vec2(1/700, 1/700));
	color += texture(buffer1, UV + vec2(2/700, 1/700));
	
	color += texture(buffer1, UV + vec2(-2/700, 0));
	color += texture(buffer1, UV + vec2(-1/700, 0));
	color += 2 * texture(buffer1, UV);
	color += texture(buffer1, UV + vec2(1/700, 0));
	color += texture(buffer1, UV + vec2(2/700, 0));
	
	color += texture(buffer1, UV + vec2(-2/700, -1/700));
	color += texture(buffer1, UV + vec2(-1/700, -1/700));
	color += texture(buffer1, UV + vec2(0, -1/700));
	color += texture(buffer1, UV + vec2(1/700, -1/700));
	color += texture(buffer1, UV + vec2(2/700, -1/700));

	color += texture(buffer1, UV + vec2(-2/700, -2/700));
	color += texture(buffer1, UV + vec2(-1/700, -2/700));
	color += texture(buffer1, UV + vec2(0, -2/700));
	color += texture(buffer1, UV + vec2(1/700, -2/700));
	color += texture(buffer1, UV + vec2(2/700, -2/700));

	color /= 26;

	color += texture(buffer2, UV) + vec4(0, texture(buffer3, UV).x, 0, 0.5);
}