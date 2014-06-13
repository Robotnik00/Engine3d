

#include "DebugState.h"


#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "glm/gtc/matrix_transform.hpp"

#include "Shaders.h"

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glext.h"

#include <ModelLoader.h>
#include <glm/gtc/type_ptr.hpp>
#include <Primitives.h>

using namespace Engine3d;

//physx::PxDefaultErrorCallback DebugState::gDefaultErrorCallback;
//physx::PxDefaultAllocator DebugState::gDefaultAllocatorCallback;









//////////////////////////////////////////////////////////////////
/// methods for DebugState:
///
//////////////////////////////////////////////////////////////////
DebugState::DebugState(Engine* engine)
    : State(engine)
{
    mEngine->SetUpdateFrequency(60.0f);
    count = 0;
    if(mPhysics.Initialize())
    {
        std::cout << "physics initialized\n";
    }

    physx::PxMaterial* physxmat = mPhysics.GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);

    physx::PxRigidStatic* groundplane = physx::PxCreatePlane(*mPhysics.GetPhysics(), physx::PxPlane(0,1,0,0), *physxmat);
    mPhysics.GetScene()->addActor(*groundplane);



    mRootNode = new SceneObjectNode();
    Engine3d::Light* light = new Light(mEngine->GetShader()->GetProgramID());
    light->SetPosition(1.0,0,0);
    light->SetDiffuse(3,3,3);
    light->SetSpecular(1,1,1);
    light->SetAmbient(.1f,.1f,.1f);
    mRootNode->AddAsset(light);
    mRootNode->Translate(glm::vec3(0.0f,-2.0f,0.0f));
    mGimble = new SceneObjectNode();
    mRootNode->AddChild(mGimble);
    mGimble->Rotate(0, glm::vec3(1,0,0));






    CreateRandomObject();



    for(int i = 0; i < 255; i++)
        mKeysDown[i] = 0;


    glClearColor(0.0, 0.0f, 0.0f, 1.0f); // Clear the background of our window to white
}

DebugState::~DebugState()
{

}

void DebugState::Update()
{
    char buffer[50];
    sprintf(buffer, "%2.2f", mEngine->GetFPS());
    mEngine->SetTitle(buffer);

    timestep++;
    if(timestep % 5 == 0)
    {
        CreateRandomObject();
    }

    mRootNode->Update();
    mPhysics.Update(1.0f/mEngine->GetUpdateFrequency());

}

void DebugState::Draw(float delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mRootNode->Draw(delta);

}

void DebugState::ProcessEvent(SDL_Event* event)
{

}

void DebugState::CreateRandomObject()
{
    int random = rand() % 2;
    if(random)
    {
        float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5;
        float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5 + 10;
        float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5 - 20;
        float l = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float w = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float h = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);



        physx::PxMaterial* physxmat = mPhysics.GetPhysics()->createMaterial(.1,.1,.1);

        SceneObjectNode* node1 = new SceneObjectNode();
        Engine3d::ModelMesh<Engine3d::SimpleVertex>* mesh1 = Engine3d::Primitives::MakeBox(l,h,w);
        mesh1->SetShader(mEngine->GetShader());
        Engine3d::Material* mat1 = new Engine3d::Material(mEngine->GetShader()->GetProgramID());
        mesh1->AddAsset(mat1);
        Engine3d::Model* model1 = new Model("box1");
        model1->AddMesh(mesh1);
        Engine3d::DrawInterface* drawmodel1 = new Engine3d::DrawModel(model1);
        node1->AddDrawInterface(drawmodel1);
        mGimble->AddChild(node1);
        physx::PxShape* geobox = mPhysics.GetPhysics()->createShape(physx::PxBoxGeometry(l/2,h/2,w/2), *physxmat);
        physx::PxRigidDynamic* boxbody = mPhysics.GetPhysics()->createRigidDynamic(physx::PxTransform(x,y,z));
        boxbody->attachShape(*geobox);
        physx::PxRigidBodyExt::updateMassAndInertia(*boxbody, 100.0f);
        node1->SetBounds(boxbody);
        char name[50];
        sprintf(name, "box%d", count);
        mPhysics.AddPhysicsObject(name, node1);
    }
    else
    {
        float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5;
        float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5 + 10;
        float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5 - 20;
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);



        physx::PxMaterial* physxmat = mPhysics.GetPhysics()->createMaterial(.1,.1,.1);

        SceneObjectNode* node1 = new SceneObjectNode();
        Engine3d::ModelMesh<Engine3d::SimpleVertex>* mesh1 = Engine3d::Primitives::MakeSphere(r,20,10);
        mesh1->SetShader(mEngine->GetShader());
        Engine3d::Material* mat1 = new Engine3d::Material(mEngine->GetShader()->GetProgramID());
        mesh1->AddAsset(mat1);
        Engine3d::Model* model1 = new Model("box1");
        model1->AddMesh(mesh1);
        Engine3d::DrawInterface* drawmodel1 = new Engine3d::DrawModel(model1);
        node1->AddDrawInterface(drawmodel1);
        mGimble->AddChild(node1);
        physx::PxShape* geobox = mPhysics.GetPhysics()->createShape(physx::PxSphereGeometry(r), *physxmat);
        physx::PxRigidDynamic* boxbody = mPhysics.GetPhysics()->createRigidDynamic(physx::PxTransform(x,y,z));
        boxbody->attachShape(*geobox);
        physx::PxRigidBodyExt::updateMassAndInertia(*boxbody, 100.0f);
        node1->SetBounds(boxbody);
        char name[50];
        sprintf(name, "sphere%d", count);
        mPhysics.AddPhysicsObject(name, node1);
    }
    count++;
    std::cout << count << std::endl;
}
