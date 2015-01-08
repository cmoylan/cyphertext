#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include <SOIL.h>

#include "OpenGL.h"


GLuint createProgram(const std::vector<GLuint> &shaderList);

GLuint createProgramFromShaders(std::string vertexFile,
				std::string fragmentFile);

GLuint createShader(GLenum shaderType, const std::string& strShaderFile);

std::string loadStringFromFile(const std::string& filename);

void loadTexture(GLuint buffer, const std::string &filename);

#endif
