#include <ModelLoader.h>
#include <Shaders.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#include <stdio.h>

using namespace Engine3d;
using namespace Assimp;


Model* ModelLoader::Load(const std::string filename, Shader* shader)
{
    Model* m = new Model(filename.data());
    Importer importer;

    const aiScene *scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll needai

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        std::string name = filename.data();
        char number[10];
        sprintf(number, "%d", i);
        name += number;
        ModelMesh<SimpleVertex>* mesh = new ModelMesh<SimpleVertex>(name);

        aiMesh* aimesh = scene->mMeshes[i];
        for(int j = 0; j < aimesh->mNumFaces; j++)
        {
            aiFace& face = aimesh->mFaces[j];
            for(int k = 0; k < 3; k++)
            {
                mesh->AddIndex(face.mIndices[k]);
            }
        }


        for(int j = 0; j < aimesh->mNumVertices; j++)
        {
            Float3f* loc = new Float3f(aimesh->mVertices[j].x, aimesh->mVertices[j].y, aimesh->mVertices[j].z);
            Float3f* norm = new Float3f(aimesh->mNormals[j].x, aimesh->mNormals[j].y, aimesh->mNormals[j].z);
            Float3f* texcoors = new Float3f(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y*-1, aimesh->mTextureCoords[0][j].z);

            mesh->AddVertex(new SimpleVertex(loc,norm,texcoors));
        }

        mesh->SetShader(shader);

        m->AddMesh(mesh);
    }



    importer.FreeScene();

    return m;
}


