
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

class Float3f : public Attribute
{
public: 
	Float3f(float x, float y, float z);
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
	SimpleVertex(Float3f* coords, Float3f* norms, Float3f* texcoords);
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
	bool mBound;
	
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

	GLuint GetIboid() { return mIboid; }
	
protected:	
	std::vector<GLshort> mData;
	GLuint mIboid;
	float mVertexThreashold;
	bool mBound;
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
	ModelMesh(aiMesh* mesh, const std::string name);
	void Draw(glm::mat4* interpolator);

	void SetVBO(VBO* vbo) { mVBO = vbo; }
	void SetIBO(IBO* ibo) { mIBO = ibo; }
	VBO* GetVBO() { return mVBO; }
	IBO* GetIBO() { return mIBO; }

	void SetShader(Shader* shader);

	void AddAsset(Asset* asset) { mAssets.push_back(asset); }
	const std::string GetName() { return mName; }
	
	aiMesh* GetMesh() { return mMesh; }

protected:

	IBO* mIBO;
	VBO* mVBO;	

	std::vector<Asset*> mAssets;

	aiMesh* mMesh;
	Shader* mShader;
	const std::string mName;
};



class Model
{
public:
	Model(const char* name) { mModelName = name; }
	void Draw(glm::mat4* interpolator);
	void AddMesh(ModelMesh* mesh) { mMeshes.push_back(mesh); }
	ModelMesh* GetMesh(int i) { return mMeshes[i]; }

	Model* Load(Shader* shader);

	const std::string GetName() { return mModelName; }
			
protected:
	std::string  mModelName;

	std::vector<ModelMesh*> mMeshes;


};




