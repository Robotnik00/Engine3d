#pragma once

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"

class Shader
{
public:
	Shader(const char* name, const char* vertexShader, const char* fragmentShader);
	
	GLuint 	GetProgramID() { return mProgramID; }
	void	MakeActive();
	void	Disable();
protected:

	GLuint LoadShader(const char* filename, char** shaderSource);

	GLuint BuildProgram(GLuint vertexShaderObject, GLuint fragmentShaderObject);
	
	const char* name;
	GLuint mProgramID;
};
