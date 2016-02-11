#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "myShader.h"

GLFWwindow* window;
const int width = 1024;
const int height = 768;

void init();

// An array of 6 vectors which represents 6 vertices
static const GLfloat g_vertex_buffer_data[] = {
				-1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f,
				-1.0f, -1.0f, 0.0f,
};

int main(){
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

		//Load plane into vertexbuffer
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);	
		glm::mat4 View = glm::lookAt(glm::vec3(4, 0, -4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //Camera matrix, pos, look at, up
		glm::mat4 Model = glm::mat4(1.0f); //model transformation
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 mvp = Projection * View * Model;
		View = glm::lookAt(glm::vec3(0, 0, -4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 mvpFront = Projection * View * Model;

		//use shader that will render the texture
		glUseProgram(createTextureShader.programID);

		//create framebuffer
		GLuint FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

		//create the texture we're going to render to
		GLuint renderedTexture;
		glGenTextures(1, &renderedTexture);
	
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
	
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// The depth buffer
		GLuint depthrenderbuffer;
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	
		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	
		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

	//Pass 2
		//use same vertexbuffer for next shader

		//create and compile the hader that ues the texture
		Shader useTextureShader;
		useTextureShader.createShader("vertUsesTexture.glsl", "fragUsesTexture.glsl");

		// Get handle for our uniform variables
		GLuint MatrixIDuseTexture = glGetUniformLocation(createTextureShader.programID, "MVP");
		GLuint texID = glGetUniformLocation(useTextureShader.programID, "renderedTexture");
		GLuint timeID = glGetUniformLocation(useTextureShader.programID, "time");


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


		//Pass 1
			// Render to our framebuffer, which rendrees to the texture attached to it
			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
			glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(createTextureShader.programID);

			//compute MVP if it can change

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			glUniformMatrix4fv(MatrixIDcreateTexture, 1, GL_FALSE, &mvpFront[0][0]);

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
			glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 6 vertices total -> 2 triangle
			glDisableVertexAttribArray(0);

		//Pass 2
			// Render to screen
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

			glUniform1f(timeID, (float)(glfwGetTime()*10.0f));

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // attribute 0. No particular reason for 0, but must match the layout in the shader // size // type // normalized? // stride // array buffer offset
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 6 vertices total -> 2 triangle
			glDisableVertexAttribArray(0);
			
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteProgram(createTextureShader.programID);
	glDeleteProgram(useTextureShader.programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteTextures(1, &renderedTexture);

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