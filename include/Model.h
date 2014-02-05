
#pragma once

#define GL_GLEXT_PROTOTYPES

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glx.h"

#include "glm/glm.hpp"

#include "assimp/Importer.hpp"

#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"

class Shader;


#include <vector>
#include <map>


// triangles
#define VERTS_PER_FACE 3

class Attribute
{
public:
	virtual void* GetData() = 0;
	virtual int GetSize() = 0; // byte size of attribute
	virtual int GetNumElements() = 0; // number of elements in the attribute
	virtual int GetType() = 0; // type of data stored
};

class FLoat3f : public Attribute
{
public: 
	FLoat3f(float x, float y, float z);
	void* GetData();
	int GetSize() { return 3*4; }
	int GetNumElements() { return 3; }
	int GetType() { return GL_FLOAT; }
protected: 
	float mX,mY,mZ;	
};

class Vertex
{
public:
	Vertex();
	void* GetData(); // to data array
	int GetSize();  // size in bytes
	int GetOffset(int index); // gets the byte offset of the attribute at index
	Attribute* GetAttribute(int i) { return mAttributes[i]; }
	int GetNumAttributes() { return mNumAttributes; }
protected:
	Attribute** mAttributes;
	int mNumAttributes;
};

class SimpleVertex : public Vertex
{
public: 
	SimpleVertex(FLoat3f* coords, FLoat3f* norms, FLoat3f* texcoords);
};

class VBO
{
public:
	VBO();
	void Load();
	void UnLoad();
	void Bind();
	void UnBind();
	void AddVertex(Vertex* vert); // returns location of vert in vbo

	void* GetData();
	int GetSize();
	int GetNumVertices() { return mData.size(); }

	GLuint GetVboid() { return mVboid; }
	
protected:	
	std::vector<Vertex*> mData;
	GLuint mVboid;
	float mVertexThreashold;
	
};

class IBO
{
public:
	IBO();
	void Load();
	void UnLoad();

	void Bind();
	void UnBind();
	void AddIndex(GLshort index); // returns location of vert in vbo

	void* GetData();
	int GetSize();
	int GetNumVertices() { return mData.size(); }

	GLuint GetVboid() { return mIboid; }
	
protected:	
	std::vector<GLshort> mData;
	GLuint mIboid;
	float mVertexThreashold;
};

class Asset
{
public:
	virtual void Bind() = 0;
	void SetProgramID(GLuint programID) { mProgramID = programID; }

protected: 
	GLuint mProgramID;
};

class Texture : public Asset
{
public:
	Texture(const char* filename) { mFilename = filename; }
	void Load();
	GLuint GetTextureID() { return mTexid; }
	void Bind();	

protected:
	std::string mFilename;
	GLuint mTexid;
};

class ModelMesh
{
public: 
	ModelMesh(aiMesh* mesh) { mMesh = mesh; }
	void Draw(glm::mat4* interpolator);

	void SetVBO(VBO* vbo) { mVBO = vbo; }
	void SetIBO(IBO* ibo) { mIBO = ibo; }

	void AddAsset(Asset* asset) { mAssets.push_back(asset); }
	
	aiMesh* GetMesh() { return mMesh; }

protected:

	IBO* mIBO;
	VBO* mVBO;	

	std::vector<Asset*> mAssets;

	aiMesh* mMesh;
};



class Model
{
public:
	Model(const char* name) { mModelName = name; }
	void Draw(glm::mat4* interpolator);
	void AddMesh(ModelMesh* mesh) { mMeshes.push_back(mesh); }
	ModelMesh* GetMesh(int i) { return mMeshes[i]; }

	Model* Load(aiScene* scene);

	const std::string GetName() { return mModelName; }
			
protected:
	std::string  mModelName;

	std::vector<ModelMesh*> mMeshes;


};




class ModelManager
{
public:

	ModelManager(Shader* shader);
	
	Model* Load(const char* modelName, const char* fileName);
	
protected:
	Shader* mShader;
	Assimp::Importer mImporter;
	std::vector<Model*> mModels;
	std::map<std::string, Model*> mMods;
};


