#include "Shaders.h"
#include <iostream>
#include <fstream>
#include <ios>

#include "GL/glu.h"
#include "GL/glext.h"




Shader::Shader(const char* name, const char* vertexShader, const char* fragmentShader)
{
	this->name = name;
	GLchar* VertexShaderSource;
	GLchar* FragmentShaderSource;

	GLint vlength = LoadShader(vertexShader, &VertexShaderSource);
	GLint flength = LoadShader(fragmentShader, &FragmentShaderSource);



	mVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	mFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSourceARB(mVertexShaderObject, 1, (const GLchar**)&VertexShaderSource, &vlength);
	glShaderSourceARB(mFragmentShaderObject, 1, (const GLchar**)&FragmentShaderSource, &flength);

	
}

GLuint Shader::LoadShader(const char* filename, char** shaderSource)
{

	std::ifstream file;
	file.open(filename, std::ios::in);
	if(!file)
	{
		return -1;
	}

	file.seekg(0, std::ios::end);
	long file_length = file.tellg();
	file.seekg(std::ios::beg);
	
	*shaderSource = new char[file_length+1];
	
	(*shaderSource)[file_length] = 0;



	
	for(int i = 0; i < file_length; i++)
	{
		(*shaderSource)[i] = file.get();
	}
	
	file.close();


	return file_length+1;
}

void Shader::MakeActive()
{
	glUseProgram(mProgramID);
}
void Shader::Disable()
{
	glUseProgram(0);
}

void Shader::RemoveMesh(ModelMesh* mesh)
{
	mMeshes.erase(mesh->GetName());
}

SimpleShader::SimpleShader(const char* name, const char* vertexShader, const char* fragmentShader)
	: Shader(name, vertexShader, fragmentShader)
{
	BuildProgram();	
}

GLuint SimpleShader::BuildProgram()
{
	glCompileShaderARB(mVertexShaderObject);
	glCompileShaderARB(mFragmentShaderObject);


	/*GLint compiled;

	glGetObjectParameterivARB(vertexShaderObject, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_TRUE)
	{
		std::cout << "vertex shader compiled\n";
	}     

	glGetObjectParameterivARB(fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_TRUE)
	{
		std::cout << "fragment shader compiled\n";
	}*/

	GLuint programID = glCreateProgram();

	glAttachShader(programID, mVertexShaderObject);
	glAttachShader(programID, mFragmentShaderObject);

	

	glBindAttribLocation(programID, 0, "VertexPosition");
	glBindAttribLocation(programID, 1, "normals");
	glBindAttribLocation(programID, 2, "tex");
	glLinkProgram(programID);
	/*glGetObjectParameterivARB(programID, GL_LINK_STATUS, &compiled);
	if (compiled == GL_TRUE)
	{
		std::cout << "program linked\n";
	}*/
	mProgramID = programID;
	return programID;
}


void SimpleShader::AddMesh(ModelMesh* modelMesh)
{
	if(mMeshes.find(modelMesh->GetName()) != mMeshes.end())
	{
		ModelMesh* tmpmesh = mMeshes[modelMesh->GetName()];
		modelMesh->SetVBO(tmpmesh->GetVBO());
		modelMesh->SetIBO(tmpmesh->GetIBO());
		std::cout << modelMesh->GetName() << " mesh loaded\n";
		return;
	}

	aiMesh *mesh = modelMesh->GetMesh();

	IBO* ibo = new IBO();

	GLshort* indices = new GLshort[mesh->mNumFaces*3];
	int index = 0;
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];			
		for(int j = 0; j < 3; j++)
		{
			indices[index++] = face.mIndices[j];
			ibo->AddIndex(face.mIndices[j]);
		}
	}


	VBO* vbo = new VBO();
	for(int i = 0; i < mesh->mNumVertices; i++)
	{
		Float3f* loc = new Float3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		Float3f* norm = new Float3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		Float3f* texcoors = new Float3f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y*-1, mesh->mTextureCoords[0][i].z);

		vbo->AddVertex(new SimpleVertex(loc,norm,texcoors));
	}
	

	vbo->Load();
	ibo->Load();



	modelMesh->SetVBO(vbo);
	modelMesh->SetIBO(ibo);
	
	mMeshes.insert(std::pair<std::string, ModelMesh*>(modelMesh->GetName(), modelMesh));
}





