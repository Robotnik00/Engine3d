
#pragma once

#define GL_GLEXT_PROTOTYPES

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glx.h"

#include "glm/glm.hpp"

#include "assimp/Importer.hpp"

#include <vector>

// used for indexing IBO and VBO
#define NORMAL_PTR 12
#define TEX_PTR	   24
#define SERERATION 36

// triangles
#define VERTS_PER_FACE 3


class Vertex
{
public:
	float x, y, z;
	float nx,ny,nz;
	float u,v;
};

class Model
{
public:
	Model(const char* name, GLuint IBO, GLuint mVBO, int numVertices);
	~Model();
	void Draw(glm::mat4* interpolator);
	
	const char*	getName()		{ return mModelName; 	}
	const int	getNumVertices()	{ return mNumVertices;	}	
			
protected:
	char*  mModelName;
	GLuint mIBO;
	GLuint mVBO;
	int mNumVertices;
};


class ModelManager
{
public:

	ModelManager(GLuint programID);
	
	static Model* Load(const char* modelName, const char* fileName);
	
protected:
	static GLuint mProgramID;
	static Assimp::Importer mImporter;
	static std::vector<Model*> mModels;
};


