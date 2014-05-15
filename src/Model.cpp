#include <Model.h>
#include <Shaders.h>

#include <string.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>


#include <SDL2/SDL.h>

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
    glBindTexture(GL_TEXTURE_2D, 0);
}

Uniform1f::Uniform1f(std::string uniname, GLuint programid)
{
  mUniId = glGetUniformLocation(programid, uniname.data());
}
void Uniform1f::SetVal(float x)
{
  mx = x;
}

void Uniform1f::Bind()
{
  glUniform1f(mUniId, mx);
}
void Uniform1f::UnBind()
{
  glUniform1f(mUniId, 0);
}

Uniform3f::Uniform3f(std::string uniname, GLuint programid)
{
    mUniId = glGetUniformLocation(programid, uniname.data());
}
void Uniform3f::SetVal(float x, float y, float z)
{
    mx = x, my =y, mz = z;
}
void Uniform3f::Bind()
{
    glUniform3f(mUniId, mx, my, mz);
}
void Uniform3f::UnBind()
{
    glUniform3f(mUniId, 0, 0, 0);
}

Uniform4f::Uniform4f(std::string uniname, GLuint programid)
{
    mUniId = glGetUniformLocation(programid, uniname.data());
}
void Uniform4f::SetVal(float x, float y, float z, float w)
{
    mx = x, my =y, mz = z, mw=w;
}
void Uniform4f::Bind()
{
    glUniform4f(mUniId, mx, my, mz, mw);
}
void Uniform4f::UnBind()
{
    glUniform4f(mUniId, 0, 0, 0, 0);
}

Material::Material(GLuint programid)
{
    mMaterialDiffuse = new Uniform3f("material.diffuse", programid);
    mMaterialDiffuse->SetVal(1,1,1);
    mMaterialSpecular = new Uniform3f("material.specular", programid);
    mMaterialSpecular->SetVal(1,1,1);
    mMaterialAmbient = new Uniform3f("material.ambient", programid);
    mMaterialAmbient->SetVal(1,1,1);
    mShininess = new Uniform1f("material.shininess", programid);
    mShininess->SetVal(50);

}
Material::~Material()
{
    delete mMaterialDiffuse;
    delete mMaterialSpecular;
    delete mMaterialAmbient;
    delete mShininess;
}

void Material::Bind()
{
    mMaterialDiffuse->Bind();
    mMaterialSpecular->Bind();
    mMaterialAmbient->Bind();
    mShininess->Bind();
}
void Material::UnBind()
{
    mMaterialDiffuse->UnBind();
    mMaterialSpecular->UnBind();
    mMaterialAmbient->UnBind();
    mShininess->UnBind();
}
Light::Light(GLuint programid)
{
    mLightPosition = new Uniform3f("light1.position", programid);
    mLightPosition->SetVal(0,0,0);
    mLightAmbient = new Uniform3f("light1.ambientLight", programid);
    mLightAmbient->SetVal(0.01f,0.01f,0.01f);
    mLightDiffuse = new Uniform3f("light1.diffuse", programid);
    mLightDiffuse->SetVal(1,1,1);
    mLightSpecular = new Uniform3f("light1.specular", programid);
    mLightSpecular->SetVal(.1,.1,.1);
    mConstAttenuation = new Uniform1f("light1.constantAttenuation", programid);
    mConstAttenuation->SetVal(1);
    mLinearAttenuation = new Uniform1f("light1.linearAttenuation", programid);
    mLinearAttenuation->SetVal(1);
    mQuadraticAttenuation = new Uniform1f("light1.quadraticAttenuation", programid);
    mQuadraticAttenuation->SetVal(1);
    mSpotCutoff = new Uniform1f("light1.spotCutoff", programid);
    mSpotCutoff->SetVal(180);
    mSpotExponent = new Uniform1f("light1.spotExponent", programid);
    mSpotExponent->SetVal(1);
    mSpotDirection = new Uniform3f("light1.spotDirection", programid);
    mSpotDirection->SetVal(0,0,-1);
}
Light::~Light()
{
    delete mLightPosition;
    delete mLightAmbient;
    delete mLightDiffuse;
    delete mLightSpecular;
    delete mConstAttenuation;
    delete mLinearAttenuation;
    delete mQuadraticAttenuation;
    delete mSpotCutoff;
    delete mSpotExponent;
    delete mSpotDirection;
}

void Light::Bind()
{
    mLightPosition->Bind();
    mLightAmbient->Bind();
    mLightDiffuse->Bind();
    mLightSpecular->Bind();
    mConstAttenuation->Bind();
    mLinearAttenuation->Bind();
    mQuadraticAttenuation->Bind();
    mSpotCutoff->Bind();
    mSpotExponent->Bind();
    mSpotDirection->Bind();
}
void Light::UnBind()
{
    mLightPosition->UnBind();
    mLightAmbient->UnBind();
    mLightDiffuse->UnBind();
    mLightSpecular->UnBind();
    mConstAttenuation->UnBind();
    mLinearAttenuation->UnBind();
    mQuadraticAttenuation->UnBind();
    mSpotCutoff->UnBind();
    mSpotExponent->UnBind();
    mSpotDirection->UnBind();
}


ModelMeshBase::ModelMeshBase(const std::string name)
	: mName(name)
{
	mVBO = NULL;
	mIBO = NULL;
}

ModelMeshBase::~ModelMeshBase()
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

void ModelMeshBase::Draw(glm::mat4* interpolator)
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
    mVBO->UnBind();
    mIBO->UnBind();
}

void ModelMeshBase::SetShader(Shader* shader)
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









