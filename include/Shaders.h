#pragma once

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "Model.h"


class Shader
{
public:
	Shader(const char* name, const char* vertexShader, const char* fragmentShader);
	
	GLuint 	GetProgramID() { return mProgramID; }
	void	MakeActive();
	void	Disable();
	void 	LoadMesh(ModelMesh* mesh);
	virtual void AddMesh(ModelMesh* modelmesh) = 0;
protected:

	GLuint LoadShader(const char* filename, char** shaderSource);

	GLuint BuildProgram(GLuint vertexShaderObject, GLuint fragmentShaderObject);
	
	
	

	const char* name;
	GLuint mProgramID;

	//std::map<std::string, Model* model> mModels;
};

class SimpleShader : public Shader
{
public:
	SimpleShader(const char* name, const char* vertexShader, const char* fragmentShader);
	void AddMesh(ModelMesh* mesh);
};



