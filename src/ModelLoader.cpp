#include <ModelLoader.h>
#include <Shaders.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#define NDEBUG
#include <PxPhysicsAPI.h>


#include <stdio.h>
#include <iostream>
using namespace Engine3d;
using namespace Assimp;

std::map<std::string, Model*> ModelLoader::mLoadedModels;


void ModelLoader::Load(Model* m, Shader* shader, PhysicsEngine* physics)
{
    if(mLoadedModels.find(m->GetFileName()) != mLoadedModels.end())
    {
        std::cout << "already loaded " << m->GetFileName() << std::endl;
        Model* tmp = mLoadedModels[m->GetFileName()];
        for(int i = 0; i < tmp->GetNumMeshes(); i++)
        {
            ModelMesh<SimpleVertex>* mesh = new ModelMesh<SimpleVertex>(tmp->GetMesh(i)->GetName());
            mesh->SetVBO(tmp->GetMesh(i)->GetVBO());
            mesh->SetIBO(tmp->GetMesh(i)->GetIBO());
            if(physics != NULL)
            {
                mesh->SetBounds(tmp->GetMesh(i)->GetBounds());
            }
            mesh->SetShader(shader);
            m->AddMesh(mesh);
        }
        return;
    }



    Importer importer;
    physx::PxMaterial* mat = physics->GetPhysics()->createMaterial(1.0f,1.0f,1.0f);
    const aiScene *scene = importer.ReadFile(m->GetFileName(), aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll needai
    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        std::string name = m->GetFileName().data();
        char number[10];
        sprintf(number, "%d", i);
        name += number;
        ModelMesh<SimpleVertex>* mesh = new ModelMesh<SimpleVertex>(name);

        std::vector<physx::PxU32> indices;
        aiMesh* aimesh = scene->mMeshes[i];
        for(int j = 0; j < aimesh->mNumFaces; j++)
        {
            aiFace& face = aimesh->mFaces[j];
            for(int k = 0; k < 3; k++)
            {
                mesh->AddIndex(face.mIndices[k]);

                indices.push_back(k);
            }
        }


        std::vector<physx::PxVec3> verts;
        for(int j = 0; j < aimesh->mNumVertices; j++)
        {
            Float3f* loc = new Float3f(aimesh->mVertices[j].x, aimesh->mVertices[j].y, aimesh->mVertices[j].z);
            Float3f* norm = new Float3f(aimesh->mNormals[j].x, aimesh->mNormals[j].y, aimesh->mNormals[j].z);
            Float3f* texcoors = NULL; 
            if(aimesh->HasTextureCoords(0))
            {
              texcoors = new Float3f(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y*-1, aimesh->mTextureCoords[0][j].z);
              
            }
            else 
            {
              texcoors = new Float3f(0,0,0);
            }
            SimpleVertex* temp = new SimpleVertex(loc,norm,texcoors);
            mesh->AddVertex(temp);
            verts.push_back(physx::PxVec3(aimesh->mVertices[j].x, aimesh->mVertices[j].y, aimesh->mVertices[j].z));
        }

        if(physics != NULL)
        {
            physx::PxConvexMeshDesc convexDesc;
            convexDesc.points.count     = verts.size();
            convexDesc.points.stride    = sizeof(physx::PxVec3);
            convexDesc.points.data      = verts.data();
            convexDesc.flags            = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX ;
            convexDesc.vertexLimit      = 256;

            physx::PxDefaultMemoryOutputStream buf;
            if(!physics->GetCooking()->cookConvexMesh(convexDesc, buf))
            {
                std::cout << "failed cooking convexmesh\n";
            }
            physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
            physx::PxConvexMesh* convexMesh = physics->GetPhysics()->createConvexMesh(input);
            physx::PxConvexMeshGeometry* geoconvex = new physx::PxConvexMeshGeometry(convexMesh);
            if(geoconvex == NULL)
            {
                std::cout << "error\n";
            }

            mesh->SetBounds(geoconvex);
        }

        mesh->SetShader(shader);



      \

        m->AddMesh(mesh);
    }



    importer.FreeScene();

    mLoadedModels[m->GetFileName()] = m;
}


