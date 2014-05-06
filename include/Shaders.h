#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <Model.h>
#include <map>
namespace Engine3d
{
    class Shader
    {
    public:
        Shader(const char* name, const char* vertexShader, const char* fragmentShader);

        GLuint 	GetProgramID() { return mProgramID; }
        void	MakeActive();
        void	Disable();
        virtual void AddMesh(ModelMeshBase* modelmesh) = 0;
        void RemoveMesh(ModelMeshBase* modelMesh);
        virtual GLuint BuildProgram() = 0;

    protected:

        GLuint LoadShader(const char* filename, char** shaderSource);




        const char* name;
        GLuint mProgramID;
        GLuint mVertexShaderObject, mFragmentShaderObject;
        std::map<std::string, ModelMeshBase*> mMeshes;
    };

    class SimpleShader : public Shader
    {
    public:
        SimpleShader(const char* name, const char* vertexShader, const char* fragmentShader);
        void AddMesh(ModelMeshBase* mesh);
        GLuint BuildProgram();

    protected:
    };



}
