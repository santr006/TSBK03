#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 vUV;
out vec4 vtex;
//extra
out vec4 estimatedPoint;
out vec3 refRay;

uniform mat4 MVP;
uniform sampler2D renderedTexture;
//extra
uniform sampler2D normals;
uniform vec3 lightPos;


void main() {
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);//grid points in light-space
	vUV = vertexUV;
	vtex = MVP * texture( renderedTexture, vUV );//water points in light-space

	//extra
	vec3 point = texture( renderedTexture, vUV ).xyz;
	vec3 normal = texture( normals, vUV ).xyz;
	
	//create a ray for every point going from the camera to the point
	vec3 r1 = point - lightPos;
	
	//get the refracted vector r2
	vec3 r2 = refract(normalize(r1), normal, 1.5);

	//Points along this ray are defined by p = start + d*r2, d = distance from start
	//initially d = 1;
	vec3 p = point + r2;

	estimatedPoint = MVP * vec4(p,1);
	refRay = r2;
}