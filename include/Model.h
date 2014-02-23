
#pragma once

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#include <vector>



class Shader;




// triangles
#define VERTS_PER_FACE 3


//////////////////////////////////////////////////////////////////////////////
/// Vertex Buffer Objects and Index Buffer Objects
//////////////////////////////////////////////////////////////////////////////
/// Individual shaders will be responsible for creating vbos and ibos specific
/// to their application. So when a mesh is added to a shader, it must contain
/// all of the information required for the shader to create these buffers.
/// And when the mesh is added, the shader will check if the mesh is already in
/// the shader in which case it will set references to the VBO and IBO needed.
//////////////////////////////////////////////////////////////////////////////




// defines an attribute in a vbo
class Attribute
{
public:
	virtual void* GetData() = 0;    // get data
	virtual int GetSize() = 0;      // byte size of attribute
	virtual int GetNumElements() = 0;   // number of elements in the attribute
	virtual int GetType() = 0;  // type of data stored
};

// a common type of attribute. just 3 floats. used to represent position,
// texture coords, normals etc...
class Float3f : public Attribute
{
public: 
	Float3f(float x, float y, float z);
	~Float3f();
	void* GetData();
	int GetSize() { return 3*4; }
	int GetNumElements() { return 3; }
	int GetType() { return GL_FLOAT; }
protected: 
	float* mData; 	
};


// defines a single vertex in a vbo.
// classes that extend from this will determine what
// attributes they have.
class Vertex
{
public:
	Vertex();
	~Vertex();
	void* GetData();    // to data array	
	int GetSize();     // size in bytes
	int GetOffset(int index);   // gets the byte offset of the attribute at index
	Attribute* GetAttribute(int i) { return mAttributes[i]; }   // gets an attribute in the vertex
	int GetNumAttributes() { return mNumAttributes; }   // number of atributes in the vertex

protected:
	void Compile();

	Attribute** mAttributes;
	int mNumAttributes;
	char* mData;
	bool mCompiled;
};


// a simple vertex that has position, normals, and texture coordinates.
// this will be a common vertex format for many basic shaders
class SimpleVertex : public Vertex
{
public: 
	SimpleVertex(Float3f* coords, Float3f* norms, Float3f* texcoords);
};



// vbos are used to efficently pass vertices and their attributes to shaders.
// be careful to only add one type of vertex to each vbo. even though it is possible
// to load different types of vertices into a vbo, this will cause errors in the shader.
class VBO
{
public:
	VBO();
	~VBO();
	void Load();    // loads vbo into graphics card memory
	void UnLoad();  // removes vbo from graphics card memory
	void Bind();    // binds to the vbo
	void UnBind();  // unbinds vbo
	void AddVertex(Vertex* vert);   // adds a vertex to vbo. if the vbo is already loaded,
                                    // than the vbo must be unloaded then reloaded with the new vertex.

	void* GetData();    // gets raw data stored in the vbo
	int GetSize();      // gets the size in bytes of the vbo
	int GetNumVertices() { return mData.size(); }

	GLuint GetVboid() { return mVboid; }
	
	bool IsBound() { return mBound; } // returns if the vbo is currently bound.

protected:	
	void Compile();
	std::vector<Vertex*> mData; // data in vbo
	GLuint mVboid;
	bool mBound;
	char* mByteData;
	bool mCompiled;	
};

// defines indices into the vbo.
class IBO
{
public:
	IBO();
	~IBO();
	void Load();    // loads ibo into graphics card memory.
	void UnLoad();  // unloads ibo from graphics card memory.

	void Bind();    // binds to the ibo
	void UnBind();  // unbinds the ibo
	void AddIndex(GLshort index); // returns location of vert in vbo

	void* GetData(); // gets the ibo data
	int GetSize();   // gets the byte size of an ibo
	int GetNumVertices() { return mData.size(); } // gets the number of ibo indices

	GLuint GetIboid() { return mIboid; } // get id
	
protected:
	void Compile();	
	std::vector<GLshort> mData; // ibo indices
	GLuint mIboid;
	bool mBound;
};

// assets are used to allow the engine to set assets specific to a single mesh.
// even tho meshes may often share the same ibo and vbo, they will also contain
// their own set of assets.(an example of an asset would be a texture, or uniform variable)
// when a mesh is being drawn it will loop through all of the assets and bind to them then
// make the draw call, and finally unbind all assets.
class Asset
{
public:
	virtual void Bind() = 0;    // bind to asset
	virtual void UnBind() = 0;  // unbind asset
	void SetProgramID(GLuint programID) { mProgramID = programID; }

protected: 
	GLuint mProgramID;
};


// used to set a texture for a mesh
class Texture : public Asset
{
public:
	Texture(const char* filename) { mFilename = filename; }
	~Texture();
	void Load();    // loads the texture into the graphics card
	void UnLoad();  // removes the texture from the graphics card
	GLuint GetTextureID() { return mTexid; }    // returns the texture id
	void Bind();    // bind to the texture.
	void UnBind();  // unbind texture

protected:
	std::string mFilename;
	GLuint mTexid;
};

// meshes are encaspulated by models. a model contains an array of meshes.
// each mesh contains a set of vertices, an a list of assets used for drawing.
// each mesh is then added to a shader.
class ModelMesh
{
public: 
	ModelMesh(aiMesh* mesh, const std::string name);
	~ModelMesh();
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

	IBO* mIBO; // a referance to a vbo(shader specific)
	VBO* mVBO; // a referance to a ibo(shader specific)

	std::vector<Asset*> mAssets; // assets to bind to before rendering

	aiMesh* mMesh; // data loaded from 3d model file
	Shader* mShader; // referance to the shader being used

	const std::string mName;	// mesh name. (name of the file + index) 
					// Ex. for file meodlname.3DS mesh name = modelname.3DS0
};


// loads a 3d model from a file and stores it into a list of 
// meshes.  Each mesh is given a default shader.	
class Model
{
public:
	Model(const char* name) { mModelName = name; }
	~Model();
	void Draw(glm::mat4* interpolator);
	void AddMesh(ModelMesh* mesh) { mMeshes.push_back(mesh); }
	ModelMesh* GetMesh(int i) { return mMeshes[i]; }

	Model* Load(Shader* shader);

	const std::string GetName() { return mModelName; }
			
protected:
	std::string  mModelName;

	std::vector<ModelMesh*> mMeshes; // list of meshes. each one gets its own assets and shader type.

};




