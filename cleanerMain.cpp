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
const int width = 20;
const int height = 20;

void init(); 
vec3 lightLookAtCalc(vec3 cameraPos, vec3 cameraLookAtVec, float nearPlane, float farPlane, vec3 lightPos);
void makeGrid(GLfloat* grid, GLfloat* texCoord, int w, int h, glm::vec3 pos, float dist, float cw, float step, int pixelW, int pixelH);

// An array of 12 vectors which represents the two triangles building the floor plane
// and the two triangles building the smaller plane above the floor that will create a shadow
static const GLfloat g_vertex_buffer_data_sand[] = {
	-2.0f, 0.0f, 2.0f,
	2.0f, 0.0f, 2.0f,
	2.0f, 0.0f, -2.0f,
	2.0f, 0.0f, -2.0f,
	-2.0f, 0.0f, -2.0f,
	-2.0f, 0.0f, 2.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
};

static const GLfloat g_vertex_buffer_data_sand_normals[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

static const GLfloat g_vertex_buffer_data_sand_UV[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	0.25f, 0.75f,
	0.75f, 0.75f,
	0.75f, 0.25f,
	0.75f, 0.25f,
	0.25f, 0.25f,
	0.25f, 0.75f,
};

static const GLfloat g_vertex_buffer_data_water[] = {
	-2.0f, 2.0f, 2.0f,
	2.0f, 2.0f, 2.0f,
	2.0f, 2.0f, -2.0f,
	2.0f, 2.0f, -2.0f,
	-2.0f, 2.0f, -2.0f,
	-2.0f, 2.0f, 2.0f,
};

static const GLfloat g_vertex_buffer_data_water_normals[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

static const GLfloat g_vertex_buffer_data_water_UV[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
};

const mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

int main(){
	vec3 cameraPosition(0, 10, -4);
	vec3 cameraLookAtPosition(0, 0, 0);
	float nearPlane = 1;
	float farPlane = 10;
	vec3 lightPosition(0, 6, 0);

	init();

	const int gridWidth = 6;
	const int gridHeight = 6;
	float step = 2;
	GLfloat theGrid[gridWidth * gridHeight * 3] = { 0 };
	GLfloat gridTexCoord[gridWidth * gridHeight * 2] = { 0 };
	GLfloat* gridPtr = theGrid;
	GLfloat* gridTexCoordPtr = gridTexCoord;
	makeGrid(gridPtr, gridTexCoordPtr, gridWidth, gridHeight, vec3(0, 0, 0), 0.0, 0.5, step, width, height);

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

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, nearPlane, farPlane);
	glm::mat4 View = glm::lookAt(cameraPosition, cameraLookAtPosition, glm::vec3(0, 0, 1)); //Camera matrix, pos, look at, up
	glm::mat4 Model = glm::mat4(1.0f); //model transformation
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model;
	glm::mat4 Light = glm::lookAt(lightPosition, cameraLookAtPosition, glm::vec3(0, 0, 1));
	glm::mat4 mvpLight = Projection * Light * Model;
	glm::mat4 mvpInvLight = glm::inverse(mvpLight);

	//use shader that will render the texture
	//glUseProgram(createTextureShader.programID);

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

	GLuint waterPointNormal_refractedRay_Texture2;
	glGenTextures(1, &waterPointNormal_refractedRay_Texture2);
	glBindTexture(GL_TEXTURE_2D, waterPointNormal_refractedRay_Texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, waterPointNormal_refractedRay_Texture2, 0);

	GLuint estimatedPointTexture3;
	glGenTextures(1, &estimatedPointTexture3);
	glBindTexture(GL_TEXTURE_2D, estimatedPointTexture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, estimatedPointTexture3, 0);

	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, DrawBuffers);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	//Shaders

	//Create and compile the shader that will create the texture
	Shader renderToTexture0Shader;
	renderToTexture0Shader.createShader("vertOnlyMoves.glsl", "fragGiveColor.glsl");
	GLuint MatrixIDcreateTexture0 = glGetUniformLocation(renderToTexture0Shader.programID, "MVP");

	Shader renderToTexture1Shader;
	renderToTexture1Shader.createShader("getPos.vert", "renderToTexture1.frag"); 
	GLuint MatrixIDcreateTexture1 = glGetUniformLocation(renderToTexture1Shader.programID, "MVP");
	GLuint MatrixInvIDcreateTexture1 = glGetUniformLocation(renderToTexture1Shader.programID, "MVPinv");

	Shader useTextureShader; 
	useTextureShader.createShader("vertUsesTexture.glsl", "passThrough2.geo", "fragUsesTexture.glsl");
	GLuint MatrixIDuseTexture = glGetUniformLocation(useTextureShader.programID, "MVP");
	GLuint TextureIDuseTexture = glGetUniformLocation(useTextureShader.programID, "renderedTexture");
	//extra
	GLuint waterNormalsTexIDuseTexture = glGetUniformLocation(useTextureShader.programID, "normals");
	GLuint lightIDuseTexture = glGetUniformLocation(useTextureShader.programID, "lightPos");
	GLuint sandTexIDuseTexture = glGetUniformLocation(useTextureShader.programID, "sand");

	/*Shader compareShader;
	createTextureShader.createShader("compareSurfaceAndGrid.vert", "compareSurfaceAndGrid.frag");
	GLuint MatrixIDcompare = glGetUniformLocation(createTextureShader.programID, "MVP");
	GLuint StepInGridIDcompare = glGetUniformLocation(createTextureShader.programID, "step");*/

	Shader seenPosShader;
	seenPosShader.createShader("returnSeenPos.vert", "returnSeenPos.frag");
	GLuint mvplightPosWIntersectID = glGetUniformLocation(seenPosShader.programID, "MVPlightPosition");
	GLuint mvplightPosInvWIntersectID = glGetUniformLocation(seenPosShader.programID, "MVPlightPositionInv");

	//Create the shader that calculates the intersection points for the photon on the geometry
	Shader intersectionShader;
	intersectionShader.createShader("calcIntersection.vert", "calcIntersection.geo", "calcIntersection.frag");
	GLuint mvpIntersectID = glGetUniformLocation(intersectionShader.programID, "MVPlightPosition");
	GLuint mvpInvIntersectID = glGetUniformLocation(intersectionShader.programID, "MVPlightPositionInv");
	GLuint lightID = glGetUniformLocation(intersectionShader.programID, "lightgPos");
	GLuint waterPosID = glGetUniformLocation(intersectionShader.programID, "waterPoint");
	GLuint waterNormID = glGetUniformLocation(intersectionShader.programID, "waterPointNormal");

	Shader intersectionShader2;
	intersectionShader2.createShader("calcIntersection2.vert", /*"passThrough.geo",*/ "calcIntersection2.frag");
	GLuint mvpIntersect2ID = glGetUniformLocation(intersectionShader2.programID, "MVPlightPosition");
	GLuint mvpInvIntersect2ID = glGetUniformLocation(intersectionShader2.programID, "MVPlightPositionInv");
	GLuint estPointIntersect2ID = glGetUniformLocation(intersectionShader2.programID, "estimatedPointOnGeometry");
	GLuint waterPointIntersect2ID = glGetUniformLocation(intersectionShader2.programID, "intersectionPointOnWater");
	GLuint refracRayIntersect2ID = glGetUniformLocation(intersectionShader2.programID, "refractedRay");

	Shader emitPointShader;
	emitPointShader.createShader("emitPoint.vert", "emitPoint.geo", "emitPoint.frag");
	GLuint MatrixIDemit = glGetUniformLocation(emitPointShader.programID, "MVP");
	GLuint texIDemit = glGetUniformLocation(emitPointShader.programID, "estimatedPoint");

	//Shader calcPointSizeShader;
	//calcPointSizeShader.createShader("calcPointSize.vert", "calcPointSize.frag");

	//main loop
	do {
		// FPS
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			//printf("%i frames/s\n", nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		//Calculate poition and lookat vector for the light source
		vec3 lightLookAtPosition = cameraLookAtPosition;//lightLookAtCalc(cameraPosition, cameraLookAtPosition, nearPlane, farPlane, lightPosition);
		//printf("%f %f %f\n", lightLookAtPosition.x, lightLookAtPosition.y, lightLookAtPosition.z);

		//Render the geometry in the scene
		//In camera-space
		/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(createTextureShader.programID);

		glUniformMatrix4fv(MatrixIDcreateTexture, 1, GL_FALSE, &mvpLight[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_POINTS, 0, 12);
		glDisableVertexAttribArray(0);*/

		//printf(renderedSceneTexture0)

		//Render the shadow map for the sand, save z-buf in tex0
		//In light-space
		/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderToTexture0Shader.programID);

		glUniformMatrix4fv(MatrixIDcreateTexture0, 1, GL_FALSE, &mvpLight[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sand);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		glDisableVertexAttribArray(0);*/

		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedSceneTexture0, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		//Render the water, save the pos in 0 = tex3
		//In light-space
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderToTexture0Shader.programID);

		glUniformMatrix4fv(MatrixIDcreateTexture0, 1, GL_FALSE, &mvpLight[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_sand);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		glDisableVertexAttribArray(0);

		//change buffer to show on screen
		/*glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, waterPointTexture1, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;*/
		//Render the water, save the pos in 0 = tex1, norm in tex2
		//In light-space
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderToTexture1Shader.programID);

		glUniformMatrix4fv(MatrixIDcreateTexture1, 1, GL_FALSE, &mvpLight[0][0]);
		glUniformMatrix4fv(MatrixInvIDcreateTexture1, 1, GL_FALSE, &mvpInvLight[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);

		//Calculate intersection points of rays with scene geometry
		//In light-space

		//change buffer to show on screen
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, estimatedPointTexture3, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		//render water texture on grid, tex0
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(useTextureShader.programID);

		glUniformMatrix4fv(MatrixIDuseTexture, 1, GL_FALSE, &mvpLight[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterPointTexture1);
		glUniform1i(TextureIDuseTexture, 0);
		//extra
		glUniform3f(lightIDuseTexture, lightPosition.x, lightPosition.y, lightPosition.z);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterPointNormal_refractedRay_Texture2);//normal
		glUniform1i(waterNormalsTexIDuseTexture, 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, renderedSceneTexture0);//sand
		glUniform1i(sandTexIDuseTexture, 2);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid_UV);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_POINTS, 0, gridWidth * gridHeight);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//---------

		//intersection with geometry, save points in tex3 and dir in tex2
		/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(intersectionShader.programID);

		glUniformMatrix4fv(mvpIntersectID, 1, GL_FALSE, &mvpLight[0][0]);
		glUniformMatrix4fv(mvpInvIntersectID, 1, GL_FALSE, &mvpInvLight[0][0]);
		glUniform3f(lightID, lightPosition.x, lightPosition.y, lightPosition.z);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterPointTexture1);
		glUniform1i(waterPosID, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterPointNormal_refractedRay_Texture2);
		glUniform1i(waterNormID, 1);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid_UV);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_POINTS, 0, gridWidth * gridHeight);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);*/


		//change buffer to show on screen
		/*glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedSceneTexture0, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;*/
		//emit
		/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(emitPointShader.programID);

		glUniformMatrix4fv(MatrixIDemit, 1, GL_FALSE, &mvpLight[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedSceneTexture0);
		glUniform1i(texIDemit, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid_UV);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_POINTS, 0, gridWidth * gridHeight);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		/*for (int i = 0; i < 1; i++)
		{
		//save point in tex3
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(intersectionShader2.programID);

		glUniformMatrix4fv(mvpIntersect2ID, 1, GL_FALSE, &mvpLight[0][0]);
		glUniformMatrix4fv(mvpInvIntersect2ID, 1, GL_FALSE, &mvpInvLight[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, estimatedPointTexture3);
		glUniform1i(estPointIntersect2ID, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterPointTexture1);
		glUniform1i(waterPointIntersect2ID, 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterPointNormal_refractedRay_Texture2);
		glUniform1i(refracRayIntersect2ID, 2);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid_UV);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_POINTS, 0, gridWidth * gridHeight);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(renderToTexture1Shader.programID);

		glUniformMatrix4fv(MatrixIDcreateTexture0, 1, GL_FALSE, &mvpLight[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_grid);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		glDisableVertexAttribArray(0);
		}*/

		//Calculate size of intersection point to be displayed on screen 
		//and filter them
		//In camera-space, on GPU?
		//rasterize shader

		//Put together the needed framebuffers with additive blending
		//In camera-space

		//Render the water surface


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	//Get the data from the texture bound to the screen
	const size_t size = width * height * 3;
	float texData[size];
	glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, texData);

	for (int i = 0; i < size; i += 3)
	{
		printf("Value at %d: %f %f %f\n", i / 3, texData[i], texData[i + 1], texData[i + 2]);
	}
	int a = 0;
	std::cin >> a;
	
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer_sand);
	glDeleteBuffers(1, &vertexbuffer_water);
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteProgram(renderToTexture0Shader.programID);
	glDeleteProgram(renderToTexture1Shader.programID);
	glDeleteProgram(useTextureShader.programID);
	glDeleteProgram(seenPosShader.programID);
	glDeleteProgram(intersectionShader.programID);
	glDeleteProgram(intersectionShader2.programID);
	glDeleteProgram(emitPointShader.programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteTextures(1, &renderedSceneTexture0); 
	glDeleteTextures(1, &waterPointTexture1);
	glDeleteTextures(1, &waterPointNormal_refractedRay_Texture2);
	glDeleteTextures(1, &estimatedPointTexture3);

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
	window = glfwCreateWindow(width, height, "Tutorial", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
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

	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_PROGRAM_POINT_SIZE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

vec3 lightLookAtCalc(vec3 cameraPos, vec3 cameraLookAtPos, float nearPlane, float farPlane, vec3 lightPos){
	//a light's look at direction should point at the center of the camera view frustum
	vec3 center = cameraPos + 0.5f * (farPlane - nearPlane) * normalize((cameraLookAtPos - cameraPos));
	vec3 dir = center - lightPos;
	return center;
}

//create grid points in the 3D-space
//create texture coordinates for the grid points
void makeGrid(GLfloat* grid, GLfloat* texCoord, int w, int h, glm::vec3 pos, float dist, float cw, float step, int pixelW, int pixelH)
{
	//give every cell a position
	glm::vec3 leftUpperCorner3D(pos.x - (float)w / 2.0f * cw + cw / 2.0f, pos.y - dist, pos.z + (float)h / 2.0f * cw - cw / 2.0f);
	//printf("leftUpperCorner = %f %f %f", leftUpperCorner.x, leftUpperCorner.y, leftUpperCorner.z);

	//assume pixelW and pixelH are even numbers, if not the error is negligible
	int centerW = pixelW/2;//350
	int centerH = pixelH / 2;
	float stepW = pixelW / w;
	float stepH = pixelH / h;
	glm::vec2 leftUpperCornerPix(centerW - w / 2 * stepW + stepW / 2, centerH + h / 2 * stepH - stepH / 2);//350 - 50*70 + 35 = 35

	int j = 0;
	for (int i = 0, k = 0; i < w*h * 3; i += 3, k += 2)
	{
		if ((i / 3) % w == 0 && i != 0)
			j++;

		glm::vec3 point = leftUpperCorner3D + ((i / 3) % w) * cw * glm::vec3(1, 0, 0) - j * cw * glm::vec3(0, 0, 1);
		grid[i] = point.x;
		grid[i + 1] = point.y;
		grid[i + 2] = point.z;

		glm::vec2 tex = leftUpperCornerPix + ((i / 3) % w) * stepW * glm::vec2(1.0, 0) - j * stepH * glm::vec2(0, 1.0);//35 + 0<99 * 70 = 35<665
		texCoord[k] = tex.x / (float)pixelW;//35<665 / 700 ~= 0<1
		texCoord[k + 1] = tex.y / (float)pixelH;

		//printf("texCoord: %f %f\n", texCoord[k], texCoord[k + 1]);
	}
}