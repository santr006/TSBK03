#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec4 estimatedPoint;
out float size;

uniform mat4 MVP;
uniform sampler2D waterPos;
uniform sampler2D normals;
uniform sampler2D sandPos;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 upperLeft;
uniform vec3 upperRight;
uniform vec3 lowerLeft;
uniform vec2 upperLeftUV;
uniform vec2 upperRightUV;
uniform vec2 lowerLeftUV;
uniform float maxSize;
uniform float minSize;
uniform float zfar;
uniform float znear;

vec2 getTextureCoordinate(vec3 w) {
	//behöver en beskrivning av light-space i world-space koordinater
	//ska jag ha canoniska koordinater? Inte till att börja med
	//light-space är i det här fallet likadant som world-space men har origo högre upp på y-axeln
	//när jag har positionen i light-space måste den konverteras till en texturkoordinat
	//kanske m h a att ha punkterna i hörnen på gridet och deras texturkoordinater

	//find the vector between the point and the light
	vec3 a = w - lightPos;

	//find the point along that vector that has y = 0
	//w.y + a.y * t = point.y = 0
	float t = -w.y / a.y;
	vec3 point = w + a * t;

	//find that points texture coordinates
	float x = (point.x - upperLeft.x) / (upperRight.x - upperLeft.x);
	float y = (point.z - lowerLeft.z) / (upperLeft.z - lowerLeft.z);
	float u = upperRightUV.x * x + upperLeftUV.x * (1 - x);
	float v = upperLeftUV.y * y + lowerLeftUV.y * (1 - y);

	return vec2(u,v);
}

void main() {
	gl_Position = vec4(vertexPosition_modelspace,1);//grid points in world-space

	vec3 point = texture( waterPos, vertexUV ).xyz;
	vec3 normal = texture( normals, vertexUV ).xyz;
	
	//create a ray for every point going from the camera to the point
	vec3 r1 = point - lightPos;
	
	//get the refracted vector r2
	vec3 r2 = refract(normalize(r1), normalize(normal), 1.5);

	//Points along this ray are defined by p = start + d*r2, d = distance from start
	//initially d = 1;
	vec3 p = point + r2;

	//project the point to texture coordinates and get the sand position
	vec2 newUV = getTextureCoordinate(p);
	p = texture( sandPos, newUV ).xyz;

	//get the distance and new estimation
	//loop
	for(int i = 0; i < 5; i++) {
		float d = length(p - point);
		p = point + d * r2;
		newUV = getTextureCoordinate(p);
		p = texture( sandPos, newUV ).xyz;
	}
	
	estimatedPoint = MVP * vec4(p,1); //in camera-space

	//calculate size of point
	float a = maxSize - zfar * (maxSize - minSize) / (zfar - znear);// 12 - 10*9/9 = 2
	float b = znear * zfar * (maxSize - minSize) / (zfar - znear);// 1 * 10 * 9 / 9 = 10
	float distToView = length(vec3(estimatedPoint.x, estimatedPoint.y, estimatedPoint.z) - cameraPos);
	size = a + b / distToView;// 2 + 10 / dist;
}

