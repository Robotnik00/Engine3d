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


	GLuint vertexShaderObject, fragmentShaderObject;	


	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSourceARB(vertexShaderObject, 1, (const GLchar**)&VertexShaderSource, &vlength);
	glShaderSourceARB(fragmentShaderObject, 1, (const GLchar**)&FragmentShaderSource, &flength);

	mProgramID = BuildProgram(vertexShaderObject, fragmentShaderObject);
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
	
	std::cout << file_length << std::endl;

	*shaderSource = new char[file_length+1];
	
	(*shaderSource)[file_length] = 0;



	
	for(int i = 0; i < file_length; i++)
	{
		(*shaderSource)[i] = file.get();
	}
	
	file.close();


	return file_length+1;
}
GLuint Shader::BuildProgram(GLuint vertexShaderObject, GLuint fragmentShaderObject)
{
	glCompileShaderARB(vertexShaderObject);
	glCompileShaderARB(fragmentShaderObject);


	GLint compiled;

	glGetObjectParameterivARB(vertexShaderObject, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_TRUE)
	{
		std::cout << "vertex shader compiled\n";
	}     

	glGetObjectParameterivARB(fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_TRUE)
	{
		std::cout << "fragment shader compiled\n";
	}

	GLuint programID = glCreateProgram();

	glAttachShader(programID, vertexShaderObject);
	glAttachShader(programID, fragmentShaderObject);

	glLinkProgram(programID);
	glGetObjectParameterivARB(programID, GL_LINK_STATUS, &compiled);
	if (compiled == GL_TRUE)
	{
		std::cout << "program linked\n";
	}

	return programID;
}


void Shader::MakeActive()
{
	glUseProgram(mProgramID);
}
void Shader::Disable()
{
	glUseProgram(0);
}

SimpleShader::SimpleShader(const char* name, const char* vertexShader, const char* fragmentShader)
	: Shader(name, vertexShader, fragmentShader)
{
}


void SimpleShader::AddMesh(ModelMesh* modelMesh)
{
	aiMesh *mesh = modelMesh->GetMesh();
	std::cout << mesh->mNumVertices  << ' ' << mesh->mNumFaces << std::endl;
	int index = 0;
	std::cout << "max num: " << mesh->mNumUVComponents[0]  << std::endl;
	GLshort* indices = new GLshort[mesh->mNumFaces*3];
	IBO* ibo = new IBO();
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
		FLoat3f* loc = new FLoat3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		FLoat3f* norm = new FLoat3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		FLoat3f* texcoors = new FLoat3f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y*-1, mesh->mTextureCoords[0][i].z);

		vbo->AddVertex(new SimpleVertex(loc,norm,texcoors));
	}
	

	vbo->Load();
	ibo->Load();



	modelMesh->SetVBO(vbo);
	modelMesh->SetIBO(ibo);
}





