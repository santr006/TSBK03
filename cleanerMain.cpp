#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "myShader.h"
#include "grid.h"

GLFWwindow* window;
const int width = 700;
const int height = 700;

void init(); 
vec3 lightLookAtCalc(vec3 cameraPos, vec3 cameraLookAtVec, float nearPlane, float farPlane, vec3 lightPos);
void makeGrid(GLfloat* grid, GLfloat* texCoord, int w, int h, glm::vec3 pos, float dist, float cw, int pixelW, int pixelH);
void printNormals(int i);
void makeSin(GLfloat* theWave, GLfloat *theNormals, int periods, float amplitude, int stepsPerPeriod);

const int sandSize = 6;
static const GLfloat g_vertex_buffer_data_sand[] = {
	-3.0f, 0.0f, 3.0f,
	3.0f, 0.0f, 3.0f,
	3.0f, 0.0f, -3.0f,
	3.0f, 0.0f, -3.0f,
	-3.0f, 0.0f, -3.0f,
	-3.0f, 0.0f, 3.0f,
	/*-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,*/
};

static const GLfloat g_vertex_buffer_data_sand_normals[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	/*0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,*/
};

static const GLfloat g_vertex_buffer_data_sand_UV[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	/*0.25f, 0.75f,
	0.75f, 0.75f,
	0.75f, 0.25f,
	0.75f, 0.25f,
	0.25f, 0.25f,
	0.25f, 0.75f,*/
};

const int waterSize = 24;
const float waterHeight = 2.f;
static const GLfloat g_vertex_buffer_data_water[] = {
	/*-2.0f, 2.0f, 2.0f,
	2.0f, 2.0f, 2.0f,
	2.0f, 2.0f, -2.0f,
	-2.0f, 2.0f, 2.0f,
	2.0f, 2.0f, -2.0f,
	-2.0f, 2.0f, -2.0f,
	/*-2.0f, 1.0f, -2.0f,
	2.0f, 2.0f, 2.0f,
	2.0f, 1.0f, -2.0f,
	-2.0f, 1.0f, -2.0f,
	-2.0f, 2.0f, 2.0f,
	2.0f, 2.0f, 2.0f,*/
	/*-2.0f, 1.0f, 0.0f,
	-2.0f, 1.0f, -2.0f,
	2.0f, 1.0f, -2.0f,
	-2.0f, 2.0f, 0.0f,
	2.0f, 1.0f, -2.0f,
	2.0f, 2.0f, 0.0f,*/

	/*1 + glm::cos(0), 2 + glm::sin(0), -2.0f,
	1 + glm::cos(3.14 / 4.0), 2 + ::sin(3.14 / 4.0), 2.0f,
	1 + glm::cos(0), 2 + glm::sin(0), 2.0f,
	1 + glm::cos(0), 2 + glm::sin(0), -2.0f,
	1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), -2.0f,
	1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), 2.0f,

	1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), -2.0f,
	1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), 2.0f,
	1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), 2.0f,
	1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), -2.0f,
	1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), -2.0f,
	1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), 2.0f,

	1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), -2.0f,
	1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), 2.0f,
	1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), 2.0f,
	1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), -2.0f,
	1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), -2.0f,
	1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), 2.0f,

	1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), -2.0f,
	1 + glm::cos(3.14), 2 + glm::sin(3.14), 2.0f,
	1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), 2.0f,
	1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), -2.0f,
	1 + glm::cos(3.14), 2 + glm::sin(3.14), -2.0f,
	1 + glm::cos(3.14), 2 + glm::sin(3.14), 2.0f,

	-1 + glm::cos(3.14), 2 + glm::sin(3.14), -2.0f,
	-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), 2.0f,
	-1 + glm::cos(3.14), 2 + glm::sin(3.14), 2.0f,
	-1 + glm::cos(3.14), 2 + glm::sin(3.14), -2.0f,
	-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(5 * 3.14 / 4.0), 2 + ::sin(5 * 3.14 / 4.0), 2.0f,

	-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), 2.0f,
	-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), 2.0f,
	-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), 2.0f,

	-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), 2.0f,
	-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), 2.0f,
	-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), 2.0f,

	-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(8 * 3.14 / 4.0), 2 + glm::sin(8 * 3.14 / 4.0), 2.0f,
	-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), 2.0f,
	-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(8 * 3.14 / 4.0), 2 + glm::sin(8 * 3.14 / 4.0), -2.0f,
	-1 + glm::cos(8 * 3.14 / 4.0), 2 + glm::sin(8 * 3.14 / 4.0), 2.0f,*/

	glm::cos(0), waterHeight + glm::sin(0), -2.0f,
	glm::cos(3.14 / 4.0), waterHeight + ::sin(3.14 / 4.0), 2.0f,
	glm::cos(0), waterHeight + glm::sin(0), 2.0f,
	glm::cos(0), waterHeight + glm::sin(0), -2.0f,
	glm::cos(3.14 / 4.0), waterHeight + glm::sin(3.14 / 4.0), -2.0f,
	glm::cos(3.14 / 4.0), waterHeight + glm::sin(3.14 / 4.0), 2.0f,
	
	glm::cos(3.14 / 4.0), waterHeight + glm::sin(3.14 / 4.0), -2.0f,
	glm::cos(3.14 / 2.0), waterHeight + glm::sin(3.14 / 2.0), 2.0f,
	glm::cos(3.14 / 4.0), waterHeight + glm::sin(3.14 / 4.0), 2.0f,
	glm::cos(3.14 / 4.0), waterHeight + glm::sin(3.14 / 4.0), -2.0f,
	glm::cos(3.14 / 2.0), waterHeight + glm::sin(3.14 / 2.0), -2.0f,
	glm::cos(3.14 / 2.0), waterHeight + glm::sin(3.14 / 2.0), 2.0f,

	glm::cos(3.14 / 2.0), waterHeight + glm::sin(3.14 / 2.0), -2.0f,
	glm::cos(3 * 3.14 / 4.0), waterHeight + glm::sin(3 * 3.14 / 4.0), 2.0f,
	glm::cos(3.14 / 2.0), waterHeight + glm::sin(3.14 / 2.0), 2.0f,
	glm::cos(3.14 / 2.0), waterHeight + glm::sin(3.14 / 2.0), -2.0f,
	glm::cos(3 * 3.14 / 4.0), waterHeight + glm::sin(3 * 3.14 / 4.0), -2.0f,
	glm::cos(3 * 3.14 / 4.0), waterHeight + glm::sin(3 * 3.14 / 4.0), 2.0f,

	glm::cos(3 * 3.14 / 4.0), waterHeight + glm::sin(3 * 3.14 / 4.0), -2.0f,
	glm::cos(3.14), waterHeight + glm::sin(3.14), 2.0f,
	glm::cos(3 * 3.14 / 4.0), waterHeight + glm::sin(3 * 3.14 / 4.0), 2.0f,
	glm::cos(3 * 3.14 / 4.0), waterHeight + glm::sin(3 * 3.14 / 4.0), -2.0f,
	glm::cos(3.14), waterHeight + glm::sin(3.14), -2.0f,
	glm::cos(3.14), waterHeight + glm::sin(3.14), 2.0f,

	/*glm::cos(3.14), waterHeight + glm::sin(3.14), -2.0f,
	glm::cos(5 * 3.14 / 4.0), waterHeight + glm::sin(5 * 3.14 / 4.0), 2.0f,
	glm::cos(3.14), waterHeight + glm::sin(3.14), 2.0f,
	glm::cos(3.14), waterHeight + glm::sin(3.14), -2.0f,
	glm::cos(5 * 3.14 / 4.0), waterHeight + glm::sin(5 * 3.14 / 4.0), -2.0f,
	glm::cos(5 * 3.14 / 4.0), waterHeight + ::sin(5 * 3.14 / 4.0), 2.0f,

	glm::cos(5 * 3.14 / 4.0), waterHeight + glm::sin(5 * 3.14 / 4.0), -2.0f,
	glm::cos(6 * 3.14 / 4.0), waterHeight + glm::sin(6 * 3.14 / 4.0), 2.0f,
	glm::cos(5 * 3.14 / 4.0), waterHeight + glm::sin(5 * 3.14 / 4.0), 2.0f,
	glm::cos(5 * 3.14 / 4.0), waterHeight + glm::sin(5 * 3.14 / 4.0), -2.0f,
	glm::cos(6 * 3.14 / 4.0), waterHeight + glm::sin(6 * 3.14 / 4.0), -2.0f,
	glm::cos(6 * 3.14 / 4.0), waterHeight + glm::sin(6 * 3.14 / 4.0), 2.0f,

	glm::cos(6 * 3.14 / 4.0), waterHeight + glm::sin(6 * 3.14 / 4.0), -2.0f,
	glm::cos(7 * 3.14 / 4.0), waterHeight + glm::sin(7 * 3.14 / 4.0), 2.0f,
	glm::cos(6 * 3.14 / 4.0), waterHeight + glm::sin(6 * 3.14 / 4.0), 2.0f,
	glm::cos(6 * 3.14 / 4.0), waterHeight + glm::sin(6 * 3.14 / 4.0), -2.0f,
	glm::cos(7 * 3.14 / 4.0), waterHeight + glm::sin(7 * 3.14 / 4.0), -2.0f,
	glm::cos(7 * 3.14 / 4.0), waterHeight + glm::sin(7 * 3.14 / 4.0), 2.0f,

	glm::cos(7 * 3.14 / 4.0), waterHeight + glm::sin(7 * 3.14 / 4.0), -2.0f,
	glm::cos(8 * 3.14 / 4.0), waterHeight + glm::sin(8 * 3.14 / 4.0), 2.0f,
	glm::cos(7 * 3.14 / 4.0), waterHeight + glm::sin(7 * 3.14 / 4.0), 2.0f,
	glm::cos(7 * 3.14 / 4.0), waterHeight + glm::sin(7 * 3.14 / 4.0), -2.0f,
	glm::cos(8 * 3.14 / 4.0), waterHeight + glm::sin(8 * 3.14 / 4.0), -2.0f,
	glm::cos(8 * 3.14 / 4.0), waterHeight + glm::sin(8 * 3.14 / 4.0), 2.0f,
	/**/
};

//cross them to get the vertex normals

static const GLfloat g_vertex_buffer_data_water_normals[] = {
	/*0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	/*0.0f, 2.0f, -1.0f,
	0.0f, 2.0f, -1.0f,
	0.0f, 2.0f, -1.0f,
	0.0f, 2.0f, -1.0f,
	0.0f, 2.0f, -1.0f,
	0.0f, 2.0f, -1.0f,*/
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	/*-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-1.17f, 2.83f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	-2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	2.83f, 1.17f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	1.17f, 2.83f, 0.0f,
	/**/
};

static const GLfloat g_vertex_buffer_data_water_UV[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
};

static const GLfloat g_vertex_buffer_quad[] = {
	1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
};

static const GLfloat g_vertex_buffer_quad_UV[] = {
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
};

const mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

int main(){
	/*int o = 0;
	printNormals(o);
	o += 9;*/

	const int stepsPerPeriod = 4;
	float amplitude = 1;
	const int periods = 1;
	GLfloat theWave[periods * stepsPerPeriod * 3] = { 0 };
	GLfloat* theWavePtr = theWave;
	GLfloat theWaveNormals[periods * stepsPerPeriod * 3] = { 0 };
	GLfloat* theWaveNormalsPtr = theWaveNormals;
	//makeSin(theWavePtr, theWaveNormalsPtr, periods, amplitude, stepsPerPeriod);
	//int WaterSize = periods * stepsPerPeriod * 3;

	vec3 cameraPosition(0, 5, -5);
	vec3 cameraLookAtPosition(0, 0, 0);
	float nearPlane = 1;
	float farPlane = 10;
	vec3 lightPosition(0, 6, 0);
	vec3 lightPositionFar(0, 30, 0);
	float maxSize = 20;
	float minSize = 10;

	init();

	const int gridWidth = 100;
	const int gridHeight = 100;
	GLfloat theGrid[gridWidth * gridHeight * 3] = { 0 };
	GLfloat gridTexCoord[gridWidth * gridHeight * 2] = { 0 };
	GLfloat* gridPtr = theGrid;
	GLfloat* gridTexCoordPtr = gridTexCoord;
	makeGrid(gridPtr, gridTexCoordPtr, gridWidth, gridHeight, vec3(0, 0, 0), 0.0, 0.035, width, height);
	
	glm::vec3 upperLeft = glm::vec3(theGrid[0], theGrid[1], theGrid[2]);
	glm::vec2 upperLeftUV = glm::vec2(gridTexCoord[0], gridTexCoord[1]);

	int i = (gridWidth - 1) * 3;
	glm::vec3 upperRight = glm::vec3(theGrid[i], theGrid[i + 1], theGrid[i + 2]);
	i = (gridWidth - 1) * 2;
	glm::vec2 upperRightUV = glm::vec2(gridTexCoord[i], gridTexCoord[i + 1]);
	
	i = gridWidth * (gridHeight - 1) * 3;
	glm::vec3 lowerLeft = glm::vec3(theGrid[i], theGrid[i + 1], theGrid[i + 2]);
	i = gridWidth * (gridHeight - 1) * 2;
	glm::vec2 lowerLeftUV = glm::vec2(gridTexCoord[i], gridTexCoord[i + 1]);

	//FPS
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	//create vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//buffer

	//Load grid into buffer
	GLuint vertexbuffer_grid;
	glGenBuffers(1, &vertexbuffer_grid);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(theGrid), theGrid, GL_STATIC_DRAW);
	GLuint vertexbuffer_grid_UV;
	glGenBuffers(1, &vertexbuffer_grid_UV);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridTexCoord), gridTexCoord, GL_STATIC_DRAW);

	//Load plane into vertexbuffer
	GLuint vertexbuffer_sand;
	glGenBuffers(1, &vertexbuffer_sand);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sand);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_sand), g_vertex_buffer_data_sand, GL_STATIC_DRAW);
	GLuint vertexbuffer_sand_normal;
	glGenBuffers(1, &vertexbuffer_sand_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sand_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_sand_normals), g_vertex_buffer_data_sand_normals, GL_STATIC_DRAW);
	GLuint vertexbuffer_sand_UV;
	glGenBuffers(1, &vertexbuffer_sand_UV);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sand_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_sand_UV), g_vertex_buffer_data_sand_UV, GL_STATIC_DRAW);

	GLuint vertexbuffer_water;
	glGenBuffers(1, &vertexbuffer_water);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_water), g_vertex_buffer_data_water, GL_STATIC_DRAW);
	GLuint vertexbuffer_water_normal;
	glGenBuffers(1, &vertexbuffer_water_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_water_normals), g_vertex_buffer_data_water_normals, GL_STATIC_DRAW);
	GLuint vertexbuffer_water_UV;
	glGenBuffers(1, &vertexbuffer_water_UV);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_water_UV), g_vertex_buffer_data_water_UV, GL_STATIC_DRAW);

	//quad
	GLuint vertexbuffer_quad;
	glGenBuffers(1, &vertexbuffer_quad);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_quad), g_vertex_buffer_quad, GL_STATIC_DRAW);
	GLuint vertexbuffer_quad_UV;
	glGenBuffers(1, &vertexbuffer_quad_UV);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_quad_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_quad_UV), g_vertex_buffer_quad_UV, GL_STATIC_DRAW);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, nearPlane, farPlane);
	glm::mat4 View = glm::lookAt(cameraPosition, cameraLookAtPosition, glm::vec3(0, 0, 1)); //Camera matrix, pos, look at, up
	glm::mat4 Model = glm::mat4(1.0f); //model transformation
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model;
	glm::mat4 Light = glm::lookAt(lightPosition, cameraLookAtPosition, glm::vec3(0, 0, 1));
	glm::mat4 mvpLight = Projection * Light * Model;
	glm::mat4 mvpInvLight = glm::inverse(mvpLight);

	//create framebuffer
	GLuint framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint renderedSceneTexture0;
	glGenTextures(1, &renderedSceneTexture0);
	glBindTexture(GL_TEXTURE_2D, renderedSceneTexture0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);//GL_RGB32F
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedSceneTexture0, 0);

	GLuint waterPointTexture1;
	glGenTextures(1, &waterPointTexture1);
	glBindTexture(GL_TEXTURE_2D, waterPointTexture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, waterPointTexture1, 0);

	GLuint waterPointNormalTexture2;
	glGenTextures(1, &waterPointNormalTexture2);
	glBindTexture(GL_TEXTURE_2D, waterPointNormalTexture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, waterPointNormalTexture2, 0);

	GLuint estimatedPointTexture3;
	glGenTextures(1, &estimatedPointTexture3);
	glBindTexture(GL_TEXTURE_2D, estimatedPointTexture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, estimatedPointTexture3, 0);

	GLuint sandPositionsTexture4;
	glGenTextures(1, &sandPositionsTexture4);
	glBindTexture(GL_TEXTURE_2D, sandPositionsTexture4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, sandPositionsTexture4, 0);

	GLuint causticsTexture5;
	glGenTextures(1, &causticsTexture5);
	glBindTexture(GL_TEXTURE_2D, causticsTexture5);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, causticsTexture5, 0);

	GLenum DrawBuffers[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
	glDrawBuffers(6, DrawBuffers);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	//Shaders

	//Create and compile the shader that will create the texture
	Shader renderDepthShader;
	renderDepthShader.createShader("vertOnlyMoves.glsl", "fragGiveDepth.glsl");
	GLuint MatrixIDDepthTexture = glGetUniformLocation(renderDepthShader.programID, "MVP");

	Shader renderToTexture0Shader;
	renderToTexture0Shader.createShader("savePos.vert", "savePos.frag");
	GLuint MatrixIDcreateTexture0 = glGetUniformLocation(renderToTexture0Shader.programID, "MVP");

	Shader renderToTexture1Shader;
	renderToTexture1Shader.createShader("getPos.vert", "renderToTexture1.frag"); 
	GLuint MatrixIDcreateTexture1 = glGetUniformLocation(renderToTexture1Shader.programID, "MVP");

	Shader useTextureShader; 
	useTextureShader.createShader("vertUsesTexture.glsl", "passThrough2.geo", "fragUsesTexture.glsl");
	GLuint MatrixIDuseTexture = glGetUniformLocation(useTextureShader.programID, "MVP");
	GLuint TextureIDuseTexture = glGetUniformLocation(useTextureShader.programID, "renderedTexture");
	GLuint waterNormalsTexIDuseTexture = glGetUniformLocation(useTextureShader.programID, "normals");
	GLuint sandTexVIDuseTexture = glGetUniformLocation(useTextureShader.programID, "sandPos");
	GLuint lightIDuseTexture = glGetUniformLocation(useTextureShader.programID, "lightPos");
	GLuint cameraIDuseTexture = glGetUniformLocation(useTextureShader.programID, "cameraPos");
	GLuint camIDuseTexture = glGetUniformLocation(useTextureShader.programID, "camPos");
	GLuint upperLeftIDuseTexture = glGetUniformLocation(useTextureShader.programID, "upperLeft");
	GLuint upperRightIDuseTexture = glGetUniformLocation(useTextureShader.programID, "upperRight");
	GLuint lowerLeftIDuseTexture = glGetUniformLocation(useTextureShader.programID, "lowerLeft");
	GLuint upperLeftUVIDuseTexture = glGetUniformLocation(useTextureShader.programID, "upperLeftUV");
	GLuint upperRightUVIDuseTexture = glGetUniformLocation(useTextureShader.programID, "upperRightUV");
	GLuint lowerLeftUVIDuseTexture = glGetUniformLocation(useTextureShader.programID, "lowerLeftUV");
	GLuint maxSizeIDuseTexture = glGetUniformLocation(useTextureShader.programID, "maxSize");
	GLuint minSizeIDuseTexture = glGetUniformLocation(useTextureShader.programID, "minSize");
	GLuint zfarIDuseTexture = glGetUniformLocation(useTextureShader.programID, "zfar");
	GLuint znearIDuseTexture = glGetUniformLocation(useTextureShader.programID, "znear"); 
	GLuint sandTexFIDuseTexture = glGetUniformLocation(useTextureShader.programID, "sand");
	GLuint sizeIDuseTexture = glGetUniformLocation(useTextureShader.programID, "sizeInPixels");

	Shader renderColorShader;
	renderColorShader.createShader("vertOnlyMoves.glsl", "fragGiveColor.glsl");
	GLuint MatrixIDColorTexture = glGetUniformLocation(renderColorShader.programID, "MVP");

	Shader combineShader;
	combineShader.createShader("combine.vert", "combine.frag");
	GLuint buffer1IDcombine = glGetUniformLocation(combineShader.programID, "buffer1");
	GLuint buffer2IDcombine = glGetUniformLocation(combineShader.programID, "buffer2");
	GLuint buffer3IDcombine = glGetUniformLocation(combineShader.programID, "buffer3");

	//main loop
	do {
		// FPS
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%i frames/s\n", nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		//Calculate position and lookat vector for the light source
		vec3 lightLookAtPosition = cameraLookAtPosition;//lightLookAtCalc(cameraPosition, cameraLookAtPosition, nearPlane, farPlane, lightPosition);
		//printf("%f %f %f\n", lightLookAtPosition.x, lightLookAtPosition.y, lightLookAtPosition.z);

		//Render the sand, save the z-buffer in tex0
		//In light-space
		glDisable(GL_BLEND);
		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedSceneTexture0, 0); //renderedSceneTexture0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, 0, 0); //waterPointTexture1
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, 0, 0); //waterPointNormalTexture2
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, 0, 0); //estimatedPointTexture3
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, 0, 0); //sandPositionsTexture4
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, 0, 0); //causticsTexture5
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderDepthShader.programID);

		glUniformMatrix4fv(MatrixIDDepthTexture, 1, GL_FALSE, &mvp[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sand);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, sandSize);
		glDisableVertexAttribArray(0);

		//Render the sand, save the pos in tex4
		//In light-space
		glDisable(GL_BLEND);
		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sandPositionsTexture4, 0); //renderedSceneTexture0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, 0, 0); //waterPointTexture1
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, 0, 0); //waterPointNormalTexture2
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, 0, 0); //estimatedPointTexture3
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, 0, 0); //sandPositionsTexture4
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, 0, 0); //causticsTexture5
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderToTexture0Shader.programID);

		glUniformMatrix4fv(MatrixIDcreateTexture0, 1, GL_FALSE, &mvpLight[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sand);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, sandSize);
		glDisableVertexAttribArray(0);

		//Render the water, save the pos in tex1, norm in tex2
		//In light-space
		glDisable(GL_BLEND);
		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, waterPointTexture1, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, waterPointNormalTexture2, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, 0, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderToTexture1Shader.programID);

		glUniformMatrix4fv(MatrixIDcreateTexture1, 1, GL_FALSE, &mvpLight[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water_normal);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, waterSize);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		//Calculate intersection points of rays with scene geometry
		//In light-space
		//render water texture on grid, emitted points tex5, 
		glEnable(GL_BLEND);
		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, causticsTexture5, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, 0, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(useTextureShader.programID);

		glUniformMatrix4fv(MatrixIDuseTexture, 1, GL_FALSE, &mvp[0][0]); 
		glUniform3f(lightIDuseTexture, lightPositionFar.x, lightPositionFar.y, lightPositionFar.z); //lightPosition
		glUniform3f(cameraIDuseTexture, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glUniform3f(camIDuseTexture, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glUniform3f(upperLeftIDuseTexture, upperLeft.x, upperLeft.y, upperLeft.z);
		glUniform3f(upperRightIDuseTexture, upperRight.x, upperRight.y, upperRight.z);
		glUniform3f(lowerLeftIDuseTexture, lowerLeft.x, lowerLeft.y, lowerLeft.z);
		glUniform2f(upperLeftUVIDuseTexture, upperLeftUV.x, upperLeftUV.y);
		glUniform2f(upperRightUVIDuseTexture, upperRightUV.x, upperRightUV.y);
		glUniform2f(lowerLeftUVIDuseTexture, lowerLeftUV.x, lowerLeftUV.y);
		glUniform1f(maxSizeIDuseTexture, maxSize);
		glUniform1f(minSizeIDuseTexture, minSize);
		glUniform1f(zfarIDuseTexture, farPlane);
		glUniform1f(znearIDuseTexture, nearPlane);
		glUniform2f(sizeIDuseTexture, width, height);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterPointTexture1);//water
		glUniform1i(TextureIDuseTexture, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterPointNormalTexture2);//normal
		glUniform1i(waterNormalsTexIDuseTexture, 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, sandPositionsTexture4);//sand position
		glUniform1i(sandTexVIDuseTexture, 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, renderedSceneTexture0);//sand depth
		glUniform1i(sandTexFIDuseTexture, 3);
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid_UV);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_POINTS, 0, gridWidth * gridHeight);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
		//render water from camera
		glDisable(GL_BLEND);
		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, estimatedPointTexture3, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, 0, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderColorShader.programID);

		glUniformMatrix4fv(MatrixIDColorTexture, 1, GL_FALSE, &mvp[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, waterSize);
		glDisableVertexAttribArray(0);

		//combine buffers
		//caustics + sand
		glEnable(GL_BLEND);
		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sandPositionsTexture4, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, 0, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(combineShader.programID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, causticsTexture5);//caustics
		glUniform1i(buffer1IDcombine, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, estimatedPointTexture3);//water color
		glUniform1i(buffer2IDcombine, 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, renderedSceneTexture0);//sand depth
		glUniform1i(buffer3IDcombine, 2);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_quad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_quad_UV);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//---------

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	//Get the data from the texture bound to the screen
	/*glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, causticsTexture5, 0);
	const size_t size = width * height * 3;
	float texData[size];
	glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, texData);

	for (int i = 0; i < size; i += 3)
	{
		printf("Value at %d: %f %f %f\n", i / 3, texData[i], texData[i + 1], texData[i + 2]);
	}*/
	int a = 0;
	std::cin >> a;
	
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer_sand);
	glDeleteBuffers(1, &vertexbuffer_water);
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteProgram(renderToTexture0Shader.programID);
	glDeleteProgram(renderToTexture1Shader.programID);
	glDeleteProgram(useTextureShader.programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteTextures(1, &renderedSceneTexture0); 
	glDeleteTextures(1, &waterPointTexture1); 
	glDeleteTextures(1, &waterPointNormalTexture2);
	glDeleteTextures(1, &estimatedPointTexture3);
	glDeleteTextures(1, &sandPositionsTexture4);
	glDeleteTextures(1, &causticsTexture5);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void init(){
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		char a;
		cin >> a;
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Underwater Caustics", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		char a;
		cin >> a;
		exit(-1);
	}

	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		char a;
		cin >> a;
		exit(-1);
	}

	//olika clearcolor för olika shaders
	//minus
	//glClearColor(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	glClearColor(0,0,0,1);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_PROGRAM_POINT_SIZE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

vec3 lightLookAtCalc(vec3 cameraPos, vec3 cameraLookAtPos, float nearPlane, float farPlane, vec3 lightPos){
	//a light's look at direction should point at the center of the camera view frustum
	vec3 center = cameraPos + 0.5f * (farPlane - nearPlane) * normalize((cameraLookAtPos - cameraPos));
	vec3 dir = center - lightPos;
	return center;
}

/*create grid points in the 3D-space
  create texture coordinates for the grid points
	GLfloat* grid - a pointer to where the grid should be saved 
	GLfloat* texCoord - a pointer to where the texture coordinates for the grid should be saved
	int gridW - number of grid points in one row in the grid
	int gridH - number of grid points in one column in the grid
	glm::vec3 pos - the position in world coordinates of the camera and the grid should be centered around it
	float dist - tha distance from the camera to place the grid 
	float cw - cell width, distance between grid points in world space
	int pixelW - number of pixels in the width of the image
	int pixelH - number of pixels in the height of the image
*/
void makeGrid(GLfloat* grid, GLfloat* texCoord, int gridW, int gridH, glm::vec3 pos, float dist, float cw, int pixelW, int pixelH)
{
	//give every cell a position
	glm::vec3 leftUpperCorner3D(pos.x - (float)gridW / 2.0f * cw + cw / 2.0f, pos.y - dist, pos.z + (float)gridH / 2.0f * cw - cw / 2.0f);
	//printf("leftUpperCorner = %f %f %f", leftUpperCorner.x, leftUpperCorner.y, leftUpperCorner.z);

	//assume pixelW and pixelH are even numbers, if not the error is negligible
	int centerW = pixelW / 2;//350
	int centerH = pixelH / 2;
	float stepW = pixelW / gridW;// 700 / 20 = 35
	float stepH = pixelH / gridH;
	glm::vec2 leftUpperCornerPix(stepW / 2, pixelH - stepH / 2);//(centerW - gridW / 2 * stepW + stepW / 2, centerH + gridH / 2 * stepH - stepH / 2);//350 - 10*35 + 35/2

	int j = 0;
	for (int i = 0, k = 0; i < gridW*gridH * 3; i += 3, k += 2)
	{
		if ((i / 3) % gridW == 0 && i != 0)
			j++;

		glm::vec3 point = leftUpperCorner3D + ((i / 3) % gridW) * cw * glm::vec3(1, 0, 0) - j * cw * glm::vec3(0, 0, 1);
		grid[i] = point.x;
		grid[i + 1] = point.y;
		grid[i + 2] = point.z;

		glm::vec2 tex = leftUpperCornerPix + ((i / 3) % gridW) * stepW * glm::vec2(1.0, 0) - j * stepH * glm::vec2(0, 1.0);//35 + 0<99 * 70 = 35<665
		texCoord[k] = tex.x / (float)pixelW;//35<665 / 700 ~= 0<1
		texCoord[k + 1] = tex.y / (float)pixelH;

		//printf("texCoord: %f %f\n", texCoord[k], texCoord[k + 1]);
	}
}

void printNormals(int i){
	glm::vec3 v1 = glm::vec3(g_vertex_buffer_data_water[i], g_vertex_buffer_data_water[i + 1], g_vertex_buffer_data_water[i + 2]);
	glm::vec3 v2 = glm::vec3(g_vertex_buffer_data_water[i + 3], g_vertex_buffer_data_water[i + 4], g_vertex_buffer_data_water[i + 5]);
	glm::vec3 v3 = glm::vec3(g_vertex_buffer_data_water[i + 6], g_vertex_buffer_data_water[i + 7], g_vertex_buffer_data_water[i + 8]);

	glm::vec3 a = v2 - v1;
	glm::vec3 b = v3 - v2;
	glm::vec3 c = v1 - v3;

	glm::vec3 n1 = glm::cross(a, -c);
	glm::vec3 n2 = glm::cross(b, -a);
	glm::vec3 n3 = glm::cross(c, -b);

	printf("n1: %f %f %f\n", n1.x, n1.y, n1.z);
	printf("n2: %f %f %f\n", n2.x, n2.y, n2.z);
	printf("n3: %f %f %f\n", n3.x, n3.y, n3.z);
}

void makeSin(GLfloat* theWave, GLfloat *theNormals, int periods, float amplitude, int stepsPerPeriod){
	//the sinus waves should be between -2 and 2

	for (int i = 0; i < periods * stepsPerPeriod; i += stepsPerPeriod) {

		for (int j = 0; j < stepsPerPeriod; j++){
			theWave[i + j] = 1 + glm::cos(j * 3.14 / stepsPerPeriod); theWave[i + j + 1] = glm::sin(j * 3.14 / stepsPerPeriod); theWave[i + j + 2] = -2;
			theWave[i + j + 3] = -2; theWave[i + 4] = 2; theWave[i + 5] = 2;
			theWave[i + 6] = glm::cos(3.14 / 4.0); theWave[i + 7] = glm::sin(3.14 / 4.0); theWave[i + 8] = 2;

			theWave[i + 3 * 3] = glm::cos(j * 3.14 / stepsPerPeriod); theWave[i + 3 * 3 + 1] = glm::sin(j * 3.14 / stepsPerPeriod); theWave[i + 3 * 3 + 2] = 2;
			theWave[i + 3 * 3 + 3] = glm::cos(j * 3.14 / stepsPerPeriod); theWave[i + 3 * 3 + 4] = glm::sin(j * 3.14 / stepsPerPeriod); theWave[i + 3 * 3 + 5] = -2;
			theWave[i + 3 * 3 + 6] = -2; theWave[i + 3 * 3 + 7] = 2; theWave[i + 3 * 3 + 8] = -2;
		}

			/*1 + glm::cos(0), 2 + glm::sin(0), -2.0f,
			1 + glm::cos(3.14 / 4.0), 2 + ::sin(3.14 / 4.0), 2.0f,
			1 + glm::cos(0), 2 + glm::sin(0), 2.0f,
			1 + glm::cos(0), 2 + glm::sin(0), -2.0f,
			1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), -2.0f,
			1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), 2.0f,

			1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), -2.0f,
			1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), 2.0f,
			1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), 2.0f,
			1 + glm::cos(3.14 / 4.0), 2 + glm::sin(3.14 / 4.0), -2.0f,
			1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), -2.0f,
			1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), 2.0f,

			1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), -2.0f,
			1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), 2.0f,
			1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), 2.0f,
			1 + glm::cos(3.14 / 2.0), 2 + glm::sin(3.14 / 2.0), -2.0f,
			1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), -2.0f,
			1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), 2.0f,

			1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), -2.0f,
			1 + glm::cos(3.14), 2 + glm::sin(3.14), 2.0f,
			1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), 2.0f,
			1 + glm::cos(3 * 3.14 / 4.0), 2 + glm::sin(3 * 3.14 / 4.0), -2.0f,
			1 + glm::cos(3.14), 2 + glm::sin(3.14), -2.0f,
			1 + glm::cos(3.14), 2 + glm::sin(3.14), 2.0f,

			-1 + glm::cos(3.14), 2 + glm::sin(3.14), -2.0f,
			-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), 2.0f,
			-1 + glm::cos(3.14), 2 + glm::sin(3.14), 2.0f,
			-1 + glm::cos(3.14), 2 + glm::sin(3.14), -2.0f,
			-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(5 * 3.14 / 4.0), 2 + ::sin(5 * 3.14 / 4.0), 2.0f,

			-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), 2.0f,
			-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), 2.0f,
			-1 + glm::cos(5 * 3.14 / 4.0), 2 + glm::sin(5 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), 2.0f,

			-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), 2.0f,
			-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), 2.0f,
			-1 + glm::cos(6 * 3.14 / 4.0), 2 + glm::sin(6 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), 2.0f,

			-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(8 * 3.14 / 4.0), 2 + glm::sin(8 * 3.14 / 4.0), 2.0f,
			-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), 2.0f,
			-1 + glm::cos(7 * 3.14 / 4.0), 2 + glm::sin(7 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(8 * 3.14 / 4.0), 2 + glm::sin(8 * 3.14 / 4.0), -2.0f,
			-1 + glm::cos(8 * 3.14 / 4.0), 2 + glm::sin(8 * 3.14 / 4.0), 2.0f,*/
	}
}