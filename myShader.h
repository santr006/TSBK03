#include <GL/glew.h>
#include <glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Shader {

public:
	GLuint programID;

	Shader();
	Shader(const char *vertexFilePath, const char *fragmentFilePath);
	Shader(const char *vertexFilePath, const char *geometryFilePath, const char *fragmentFilePath);
	~Shader();

	void createShader();
	void createShader(const char *vertexFilePath, const char *fragmentFilePath);
	void createShader(const char *vertexFilePath, const char *geometryFilePath, const char *fragmentFilePath);


private:
	string readFile(const char *filePath);

};
