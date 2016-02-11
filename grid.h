#pragma once
#include <glm\glm.hpp>
#include <GL/glew.h>
#include <stdio.h>

class Grid
{
public:
	Grid(int w, int h, int pointsOnW, int pointsOnH) : width(w), height(h), noPointsOnWidth(pointsOnW), noPointsOnHeight(pointsOnH){};
	~Grid();

	void compareGridToTexture(GLfloat* texture, int w, int h, GLfloat* result);
	void compareGridToTexture(glm::vec4* texture, int w, int h, glm::vec3* normals, glm::vec4* resultPos, glm::vec3* resultNorm);

	int width; 
	int height;
	int noPointsOnWidth;
	int noPointsOnHeight;

};