# version 330

//gives a first estimation of intersection point for a photon (in world-space)
//and the direction the photon is traveling in

layout(location = 3) out vec4 estimatedIntersectionPointInWorldSpace;
layout(location = 2) out vec3 refractedRay;

in vec3 fdir;
in vec2 fUV;

uniform mat4 MVPlightPositionInv;
uniform sampler2D waterPoint;
uniform sampler2D waterPointNormal;

void main(){

	//get the refracted vector r2
	vec3 N = texture(waterPointNormal, fUV).xyz;
	vec3 r2 = refract(normalize(fdir), N, 1.5);

	//Points along this ray are defined by p = start + d*r2, d = distance from start
	//initially d = 1;
	vec3 p = gl_FragCoord.xyz + r2;
	
	//transform the intersection points to world-space
	estimatedIntersectionPointInWorldSpace = MVPlightPositionInv * vec4(p,1);

	//refractedRay = r2;
}