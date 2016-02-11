# version 330

layout(location = 1) out vec4 waterInterectionPoint;
layout(location = 2) out vec3 waterInterectionPointNormal;

in vec3 normals;

uniform sampler2D gridpoint;

void main(){
	
	waterInterectionPoint = vec4(0,0,0,0);
	if(gl_FragCoord.y % step == 0)
	{
		if(gl_FragCoord.x % step == 0)
		{
			waterInterectionPoint = texture(pointsOnWater, gl_FragCoord.xy);
			waterInterectionPoint.w = 1;
			waterInterectionPointNormal = normals;
		}
	}
	
}