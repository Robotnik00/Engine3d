#include <Model.h>
#include <Shaders.h>

#include <string.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>


#include <SDL.h>
#include <SDL_opengl.h>

using namespace Engine3d;

Float3f::Float3f(float x, float y, float z)
{
	mData = new float[3];
	mData[0] = x;
	mData[1] = y;
	mData[2] = z;
}
Float3f::~Float3f()
{
	delete[] mData;
}
void* Float3f::GetData()
{
	return (void*)mData;
}

Vertex::Vertex()
{
	mAttributes = NULL;
	mNumAttributes = 0;
	mData = NULL;
	mCompiled = false;
}
Vertex::~Vertex()
{
	delete[] mData;
	for(int i = 0; i < mNumAttributes; i++)
	{
		delete mAttributes[i];
	}
	delete[] mAttributes;
}

void* Vertex::GetData()
{
	if(!mCompiled)
		Compile();
	return mData;
}

int Vertex::GetSize()
{
	int size = 0;
	for(int i = 0; i < mNumAttributes; i++)
	{
		size += mAttributes[i]->GetSize();
	}
	return size;
}

int Vertex::GetOffset(int index)
{
	if(index > mNumAttributes-1)
	{
		return -1;
	}

	int o = 0;
	for(int i = 0; i < index; i++)
	{
		o += mAttributes[i]->GetSize();
	}
	return o;
}
void Vertex::Compile()
{
	if(mData != NULL)
	{
		delete[] mData;
	}
	int index = 0;
	int size = GetSize();
	mData = new char[size];
	for(int i = 0; i < mNumAttributes; i++)
	{
		char* tdata = ((char*)mAttributes[i]->GetData());
		for(int j = 0; j < mAttributes[i]->GetSize(); j++)
		{
			mData[index++] = tdata[j];
		}
        //delete[] tdata;
	}

	mCompiled = true;

}

SimpleVertex::SimpleVertex(Float3f* coords, Float3f* norms, Float3f* texcoords)
{
	mNumAttributes = 3;
	mAttributes = new Attribute*[mNumAttributes];
	mAttributes[0] = coords;
	mAttributes[1] = norms;
	mAttributes[2] = texcoords;
}

VBO::VBO()
{
	mVboid = -1;
	mBound = false;
	mByteData = NULL;
	mCompiled = false;
}

VBO::~VBO()
{
	for(int i = 0; i < mData.size(); i++)
	{
		delete mData[i];
	}
}

void VBO::AddVertex(Vertex* vert)
{
	mData.push_back(vert);
	mCompiled = false;
}

int VBO::GetSize()
{
	int size = 0;
	for(int i = 0; i < mData.size(); i++)
	{
		size += mData[i]->GetSize();
	}
	return size;
}

void* VBO::GetData()
{
	if(mCompiled != true)
		Compile();

	return mByteData;
}


void VBO::Load()
{
	char* data = (char*)GetData();
	glGenBuffersARB(1, &mVboid);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboid);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, GetSize(), data, GL_STATIC_DRAW_ARB);
	delete[] data;
}

void VBO::UnLoad()
{
	glDeleteBuffers(1, &mVboid);
}

void VBO::Compile()
{
	if(mByteData != NULL)
	{
		delete[] mByteData;
	}
	mByteData = new char[GetSize()];
	int index = 0;
	for(int i = 0; i < mData.size(); i++)
	{
		char* tdata = ((char*)mData[i]->GetData());
		for(int j = 0; j < mData[i]->GetSize(); j++)
		{
			mByteData[index++] = tdata[j];
		}
		delete[] tdata;
	}
	mCompiled = true;
}

void VBO::Bind()
{

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboid);

	for(int i = 0; i < mData[0]->GetNumAttributes(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, VERTS_PER_FACE, mData[0]->GetAttribute(i)->GetType(), GL_FALSE, mData[0]->GetSize(), (void *) mData[0]->GetOffset(i));
	}
	mBound = true;
}
void VBO::UnBind()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	mBound = false;
}

IBO::IBO()
{
	mBound = false;
	mIboid = -1;
}
IBO::~IBO()
{

	UnLoad();
}
void IBO::Load()
{
	char* data = (char*)GetData();
	glGenBuffersARB(1, &mIboid);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, mIboid);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, GetSize(), data, GL_STATIC_DRAW_ARB);
	delete[] data;
}

void IBO::UnLoad()
{
	glDeleteBuffers(1, &mIboid);
}

void IBO::Bind()
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, mIboid);
}

void IBO::UnBind()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void IBO::AddIndex(GLshort index)
{
	mData.push_back(index);
}

int IBO::GetSize()
{
	return mData.size()*sizeof(GLshort);
}

void* IBO::GetData()
{
	return (void*)&mData[0];
}
Texture::~Texture()
{
	glDeleteTextures(1, &mTexid);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, mTexid);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}
void Texture::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Load()
{

	SDL_Surface* surface = SDL_LoadBMP(mFilename.data());
	glGenTextures(1, &mTexid);    
	glBindTexture(GL_TEXTURE_2D,mTexid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w,surface->h, 0, GL_RGB,GL_UNSIGNED_BYTE,surface->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	SDL_FreeSurface(surface);
}
ModelMesh::ModelMesh(const std::string name)
	: mName(name)
{
	mVBO = NULL;
	mIBO = NULL;
}

ModelMesh::~ModelMesh()
{
	for(int i = 0; i < mAssets.size(); i++)
	{
		delete mAssets[i];
	}
    for(int i = 0; i < mVertices.size(); i++)
    {
        delete mVertices[i];
    }
}

void ModelMesh::Draw(glm::mat4* interpolator)
{
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(*interpolator));
	for(int i = 0; i < mAssets.size(); i++)
	{
		mAssets[i]->Bind();
	}
	mVBO->Bind();
	mIBO->Bind();
	glDrawElements(GL_TRIANGLES, mIBO->GetNumVertices(), GL_UNSIGNED_SHORT, (void*)0);
	glPopMatrix();

    for(int i = 0; i < mAssets.size(); i++)
    {
        mAssets[i]->UnBind();
    }
}

void ModelMesh::SetShader(Shader* shader)
{
	mShader = shader;	
	shader->AddMesh(this);
}


void Model::Draw(glm::mat4* interpolator)
{
	glPushMatrix();
	for(int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i]->Draw(interpolator);
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glPopMatrix();
}
Model::~Model()
{
	for(int i = 0; i < mMeshes.size(); i++)
	{
		delete mMeshes[i];
	}
}









