#include "Model.h"
#include "Shaders.h"

#include <string.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


#include <SDL.h>
#include <SDL_opengl.h>

FLoat3f::FLoat3f(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

void* FLoat3f::GetData()
{
	float* data = new float[3];
	data[0] = mX;
	data[1] = mY;
	data[2] = mZ;
	

	return (void*)data;
}

Vertex::Vertex()
{
	mAttributes = NULL;
	mNumAttributes = 0;
}

void* Vertex::GetData()
{
	int size = GetSize();

	char* data = new char[size];
	int index = 0;
	for(int i = 0; i < mNumAttributes; i++)
	{
		for(int j = 0; j < mAttributes[i]->GetSize(); j++)
		{
			data[index++] = ((char*)mAttributes[i]->GetData())[j];
		}
	}

	return data;
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

SimpleVertex::SimpleVertex(FLoat3f* coords, FLoat3f* norms, FLoat3f* texcoords)
{
	mNumAttributes = 3;
	mAttributes = new Attribute*[mNumAttributes];
	mAttributes[0] = coords;
	mAttributes[1] = norms;
	mAttributes[2] = texcoords;
}

VBO::VBO()
{
	mVertexThreashold = 0.001;
	mVboid = -1;
}

void VBO::AddVertex(Vertex* vert)
{
	mData.push_back(vert);
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
	char* data = new char[GetSize()];

	int index = 0;
	for(int i = 0; i < mData.size(); i++)
	{
		for(int j = 0; j < mData[i]->GetSize(); j++)
		{
			data[index++] = ((char*)mData[i]->GetData())[j];
		}
	}


	return data;
}


void VBO::Load()
{
	glGenBuffersARB(1, &mVboid);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboid);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, GetSize(), GetData(), GL_STATIC_DRAW_ARB);

}

void VBO::UnLoad()
{
	glDeleteBuffers(1, &mVboid);
}

void VBO::Bind()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVboid);

	for(int i = 0; i < mData[0]->GetNumAttributes(); i++)
	{
		glVertexAttribPointer(i, VERTS_PER_FACE, mData[0]->GetAttribute(i)->GetType(), GL_FALSE, mData[0]->GetSize(), (void *) mData[0]->GetOffset(i));
	}
}
void VBO::UnBind()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

IBO::IBO()
{
	mIboid = -1;
}
void IBO::Load()
{
	glGenBuffersARB(1, &mIboid);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, mIboid);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, GetSize(), GetData(), GL_STATIC_DRAW_ARB);
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
void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, mTexid);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}
void Texture::Load()
{

	SDL_Surface* surface = SDL_LoadBMP(mFilename.data());
	if(surface == NULL)
	{
		std::cout << "error loading texture\n";
	}
	glGenTextures(1, &mTexid);    
	glBindTexture(GL_TEXTURE_2D,mTexid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w,surface->h, 0, GL_RGB,GL_UNSIGNED_BYTE,surface->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	SDL_FreeSurface(surface);

	std::cout << mFilename << "  " << mTexid << std::endl;
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


ModelManager::ModelManager(Shader* shader)
{
	mShader = shader;
}

// what still needs to be done:
//			right now each model gets its own vertex buffer object,
//			it would be more efficent to have all IBOs index
//			a single VBO.
Model* ModelManager::Load(const char* name, const char* filename)
{
	if(mMods.find(name) != mMods.end())
	{
		std::cout << "model already loaded\n";
		return mMods[std::string(name)];
	}
	
	Model* mod = new Model(name);
	


	const aiScene *scene = mImporter.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll needai
	

	for(int i = 0; i < scene->mNumMeshes; i++)
	{

		std::cout << "number of textures: " << scene->mNumTextures << std::endl;


		ModelMesh* mesh = new ModelMesh(scene->mMeshes[i]);

		mShader->AddMesh(mesh);

		mod->AddMesh(mesh);

	
	}	
	
	mMods.insert(std::pair<std::string, Model*>(mod->GetName(), mod));
	std::cout << "done\n";
	return mod;
}











