#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 vUV;
out vec4 vtex;

uniform mat4 MVP;
uniform sampler2D renderedTexture;

void main() {
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
	vUV = vertexUV;
	vtex = MVP * texture( renderedTexture, vUV );
}