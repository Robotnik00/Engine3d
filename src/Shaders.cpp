#include <Shaders.h>
#include <iostream>
#include <fstream>
#include <ios>



using namespace Engine3d;

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

void Shader::RemoveMesh(ModelMeshBase* mesh)
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


void SimpleShader::AddMesh(ModelMeshBase* modelMesh)
{
	if(mMeshes.find(modelMesh->GetName()) != mMeshes.end())
	{
        ModelMeshBase* tmpmesh = mMeshes[modelMesh->GetName()];
		modelMesh->SetVBO(tmpmesh->GetVBO());
		modelMesh->SetIBO(tmpmesh->GetIBO());
		std::cout << modelMesh->GetName() << " mesh loaded\n";
		return;
	}


    VBO* vbo = new VBO();
    std::vector<Vertex*> verts = modelMesh->GetVertices();
    for(int i = 0; i < verts.size(); i++)
    {
        vbo->AddVertex(verts[i]);
    }

    IBO* ibo = new IBO();
    std::vector<unsigned int> indices = modelMesh->GetIndices();
    for(int i = 0; i < indices.size(); i++)
    {
        ibo->AddIndex(indices[i]);
    }

    vbo->Load();
    ibo->Load();


	modelMesh->SetVBO(vbo);
	modelMesh->SetIBO(ibo);

    mMeshes[modelMesh->GetName()] = modelMesh;


}





