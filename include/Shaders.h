#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <Model.h>
#include <map>

class Shader
{
public:
	Shader(const char* name, const char* vertexShader, const char* fragmentShader);
	
	GLuint 	GetProgramID() { return mProgramID; }
	void	MakeActive();
	void	Disable();
	virtual void AddMesh(ModelMesh* modelmesh) = 0;
	void RemoveMesh(ModelMesh* modelMesh);
	virtual GLuint BuildProgram() = 0;
	
protected:

	GLuint LoadShader(const char* filename, char** shaderSource);

	
	

	const char* name;
	GLuint mProgramID;
	GLuint mVertexShaderObject, mFragmentShaderObject;
	std::map<std::string, ModelMesh*> mMeshes;
};

class SimpleShader : public Shader
{
public:
	SimpleShader(const char* name, const char* vertexShader, const char* fragmentShader);
	void AddMesh(ModelMesh* mesh);
	GLuint BuildProgram();
	
protected:
};



