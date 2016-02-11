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

// An array of 12 vectors which represents the two triangles building the floor plane
// and the two triangles building the smaller plane above the floor that will create a shadow
static const GLfloat g_vertex_buffer_data[] = {
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
				/*-2.0f, 0.0f, 2.0f,
				-2.0f, 0.0f, 3.0f,
				-3.0f, 0.0f, 3.0f,
				-2.0f, 0.0f, 2.0f,
				-3.0f, 0.0f, 3.0f,
				-3.0f, 0.0f, 2.0f,*/
};

const mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

int main(){
	vec3 cameraPosition(0, 5, -4);
	vec3 cameraLookAtPosition(0, 0, 0);
	float nearPlane = 1;
	float farPlane = 10;
	vec3 lightPosition(0, 6, 0);

	init();

	//FPS
		double lastTime = glfwGetTime();
		int nbFrames = 0;

	//create vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//Pass 1
		//Create and compile the shader that will create the texture
		Shader createTextureShader;
		createTextureShader.createShader("vertOnlyMoves.glsl", "fragGiveColor.glsl");

		// Get a handle for our "MVP" uniform
		GLuint MatrixIDcreateTexture = glGetUniformLocation(createTextureShader.programID, "MVP");
		GLuint arrayID = glGetUniformLocation(createTextureShader.programID, "anArray");


		//Load plane into vertexbuffer
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, nearPlane, farPlane);	
		glm::mat4 View = glm::lookAt(cameraPosition, cameraLookAtPosition, glm::vec3(0, 0, 1)); //Camera matrix, pos, look at, up
		glm::mat4 Model = glm::mat4(1.0f); //model transformation
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 mvp = Projection * View * Model;
		glm::mat4 Light = glm::lookAt(lightPosition, cameraLookAtPosition, glm::vec3(0, 0, 1));
		glm::mat4 mvpLight = Projection * Light * Model;

		//use shader that will render the texture
		glUseProgram(createTextureShader.programID);

		//create framebuffer
		GLuint FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		
		/*//create the texture we're going to render to
		GLuint renderedTexture;
		glGenTextures(1, &renderedTexture);	
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, renderedTexture);	
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);	
		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

		// The depth buffer
		/*GLuint depthrenderbuffer;
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
		
		// Alternative : Depth texture. Slower, but you can sample it later in your shader
		GLuint depthTexture;
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, 1024, 768, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

		// Set "renderedTexture" as our colour attachement #0
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
		//glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	
		// Set the list of draw buffers.
		//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		//glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		GLuint depthTexture;
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

		glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	
		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

	//Pass 2
		//use same vertexbuffer for next shader

		/*//create and compile the hader that ues the texture
		Shader useTextureShader;
		useTextureShader.createShader("vertUsesTexture.glsl", "fragUsesTexture.glsl");

		// Get handle for our uniform variables
		GLuint MatrixIDuseTexture = glGetUniformLocation(createTextureShader.programID, "MVP");
		GLuint MatrixIDdepthUseTexture = glGetUniformLocation(createTextureShader.programID, "depthBiasMVP");
		GLuint depthID = glGetUniformLocation(createTextureShader.programID, "depthTex");
		GLuint texID = glGetUniformLocation(useTextureShader.programID, "shadowMap");
		GLuint timeID = glGetUniformLocation(useTextureShader.programID, "time");*/

		/*Shader shadowShader;
		shadowShader.createShader("ShadowMapping_SimpleVersion.vertexshader", "ShadowMapping_SimpleVersion.fragmentshader");

		// Get a handle for our "myTextureSampler" uniform
		GLuint TextureID = glGetUniformLocation(shadowShader.programID, "myTextureSampler");

		// Get a handle for our "MVP" uniform
		GLuint MatrixID = glGetUniformLocation(shadowShader.programID, "MVP");
		GLuint DepthBiasID = glGetUniformLocation(shadowShader.programID, "DepthBiasMVP");
		GLuint ShadowMapID = glGetUniformLocation(shadowShader.programID, "shadowMap");*/

		//Create the shader that calculates the intersection points for the photon on the geometry
		Shader intersectionShader;
		intersectionShader.createShader("calcIntersection.vert", "calcIntersection.frag");
		// Get a handles for the uniform variables
		GLuint mvpIntersectID = glGetUniformLocation(intersectionShader.programID, "MVP");
		//GLuint arrayIntersectID = glGetUniformLocation(intersectionShader.programID, "anArray");




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

		//Calculate poition and lookat vector for the light source
			vec3 lightLookAtPosition = cameraLookAtPosition;//lightLookAtCalc(cameraPosition, cameraLookAtPosition, nearPlane, farPlane, lightPosition);
			//printf("%f %f %f\n", lightLookAtPosition.x, lightLookAtPosition.y, lightLookAtPosition.z);
		
		//Render the shadow map
		//In light-space
			// Render to our framebuffer, which renders to the texture attached to it
			/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FramebufferName);
			glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(createTextureShader.programID);

			//compute MVP if it can change
			glm::mat4 Light = glm::lookAt(lightPosition, lightLookAtPosition, glm::vec3(0, 0, 1));
			glm::mat4 mvpLight = Projection * Light * Model;

			float hej[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			glUniformMatrix4fv(MatrixIDcreateTexture, 1, GL_FALSE, &mvpLight[0][0]);
			glUniform3fv(arrayID, 2, &hej[0]);

			//Enable and bind vertexbuffers
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 12 vertices total -> 4 triangle
			glDisableVertexAttribArray(0);*/

		//Render a mask to represent the water surface
		//In light-space, optional

		//Render the geometry in the scene
		//In camera-space
			// Render to our framebuffer, which renders to the texture attached to it
			/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FramebufferName);
			glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			/*glUseProgram(useTextureShader.programID);

			//compute MVP if it can change
			mat4 mvpDepth = biasMatrix * mvpLight;

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			glUniformMatrix4fv(MatrixIDuseTexture, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(MatrixIDdepthUseTexture, 1, GL_FALSE, &mvpDepth[0][0]);*/

			/*glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

			// Use our shader
			glUseProgram(shadowShader.programID);

			glm::mat4 biasMatrix(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
				);

			glm::mat4 depthBiasMVP = biasMatrix*mvpLight;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			// Set our "myTextureSampler" sampler to user Texture Unit 0
			glUniform1i(TextureID, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glUniform1i(ShadowMapID, 1);


			//Enable and bind vertexbuffers
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			//glUniform1i(texID, 0);
			//glUniform1i(depthID, 0);
			//glUniform1f(timeID, (float)(glfwGetTime()*10.0f));
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 12 vertices total -> 4 triangle
			glDisableVertexAttribArray(0);*/

		//optional reflection and refraction rendering pass

		//Calculate intersection points of rays with scene geometry
		//In light-space
			//create photon grid in light-space as an array of points
			//should all cells be the same distance from thë light source so that the plane i curved around the light source?
			//Grid photonGrid(200, 200, lightPosition, 2, 0.01f);//jobbig beräkning

			//send a low resolution grid to the GPU to be rendered
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(intersectionShader.programID);

			//compute MVP if it can change
			//glm::mat4 Light = glm::lookAt(lightPosition, lightLookAtPosition, glm::vec3(0, 0, 1));
			//glm::mat4 mvpLight = Projection * Light * Model;

			//create array to send to GPU
			//float hej[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			glUniformMatrix4fv(mvpIntersectID, 1, GL_FALSE, &mvpLight[0][0]);
			//glUniform3fv(arrayIntersectID, 2, &hej[0]);

			//Enable and bind vertexbuffers
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 12 vertices total -> 4 triangle
			glDisableVertexAttribArray(0);

		//Calculate size of intersection point to be displayed on screen 
		//and filter them
		//In camera-space, on GPU?
			//transform the intersection points to camera-space

			//Point closer to the camera should be bigger on the screen
			//calculate each point's ize with s = a + b/dc, dc = distance from camera,
			// a = smax - zfar*(smax-smin)/(zfar-znear), b = znear*zfar*(smax-smin)/(zfar-znear)

			//rasterize points in a high accuracy render target(fbo?)
			//z-test the points against the camera's depth buffer

			//calculate final intensity with I = Iphoton*e^(-g*ds), g = konstant for intensity fading in the medium, ds = distance from surface

		//Godrays
		//Calculate intersection points with the water surface
		//In light-space

		//Calculate how big the godrays should be displayed on the screen
		//In camera-space

		//Filter godrays
		//In camera-space

		//Put together the needed framebuffers with additive blending
		//In camera-space

		//Render the water surface


		//Pass 1
			/*// Render to our framebuffer, which rendrees to the texture attached to it
			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
			glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(createTextureShader.programID);

			//compute MVP if it can change

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			glUniformMatrix4fv(MatrixIDcreateTexture, 1, GL_FALSE, &mvpLight[0][0]);

			//Enable and bind vertexbuffers
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 12 vertices total -> 4 triangle
			glDisableVertexAttribArray(0);*/

		//Pass 2
			// Render to screen
			/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(useTextureShader.programID);

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			glUniformMatrix4fv(MatrixIDuseTexture, 1, GL_FALSE, &mvp[0][0]);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderedTexture);
			// Set our "renderedTexture" sampler to user Texture Unit 0
			glUniform1i(texID, 0);
			glUniform1i(depthID, 0);

			glUniform1f(timeID, (float)(glfwGetTime()*10.0f));

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // attribute 0. No particular reason for 0, but must match the layout in the shader // size // type // normalized? // stride // array buffer offset
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 12 vertices total -> 4 triangle
			glDisableVertexAttribArray(0);*/
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteProgram(createTextureShader.programID);
	glDeleteProgram(intersectionShader.programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteTextures(1, &depthTexture);

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