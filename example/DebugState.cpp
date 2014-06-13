

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
    count = 0;
    if(mPhysics.Initialize())
    {
        std::cout << "physics initialized\n";
    }

    physx::PxMaterial* physxmat = mPhysics.GetPhysics()->createMaterial(0.5f, 0.5f, 0.1f);

    mRootNode = new SceneObjectNode();
    mLight = new Light(mEngine->GetShader()->GetProgramID());
    mLight->SetPosition(1.0,0,0);
    mLight->SetDiffuse(3,3,3);
    mLight->SetSpecular(1,1,1);
    mLight->SetAmbient(.1f,.1f,.1f);
    mRootNode->AddAsset(mLight);
    mRootNode->Translate(glm::vec3(0,-2.5,-5));
    mRootNode->Rotate(15, glm::vec3(1,0,0));

    ModelMesh<SimpleVertex>* spheremesh1 = Primitives::MakeBox(1.0f, 1.0f, 1.0f);
    spheremesh1->SetShader(mEngine->GetShader());
    Material* material1 = new Material(mEngine->GetShader()->GetProgramID());
    material1->SetDiffuse(1,0,0);
    material1->SetAmbient(1,0,0);
    material1->SetSpecular(0,1,0);
    material1->SetShininess(50.0f);
    spheremesh1->AddAsset(material1);
    mModel1 = new Model("sphere1");
    mModel1->AddMesh(spheremesh1);
    mNode1 = new SceneObjectNode();
    DrawModel* drawSphere1 = new DrawModel(mModel1);
    mNode1->AddDrawInterface(drawSphere1);
    mNode1->SetLocalPosition(glm::vec3(-1.0f,5.0f,-10.0f));
    mRootNode->AddChild(mNode1);
    physx::PxTransform transform1(-1.5f,5.0f,-10.5f);
    physx::PxBoxGeometry geosphere1 = physx::PxBoxGeometry(1.0f,1.0f,1.0f);
    physx::PxRigidDynamic* spherebody = mPhysics.GetPhysics()->createRigidDynamic(transform1);
    spherebody->attachShape(*mPhysics.GetPhysics()->createShape(geosphere1, *physxmat));
    physx::PxRigidBodyExt::updateMassAndInertia(*spherebody, 1.0f);
    mNode1->SetBounds(spherebody);
    mPhysics.AddPhysicsObject("box", mNode1);


    ModelMesh<SimpleVertex>* spheremesh2 = Primitives::MakeSphere(1.0f, 20.0f, 10.0f);
    spheremesh2->SetShader(mEngine->GetShader());
    Material* material2 = new Material(mEngine->GetShader()->GetProgramID());
    material2->SetDiffuse(0,1,0);
    material2->SetAmbient(0,1,0);
    material2->SetSpecular(0,1,0);
    material2->SetShininess(50.0f);
    spheremesh2->AddAsset(material2);
    mModel2 = new Model("sphere2");
    mModel2->AddMesh(spheremesh2);
    mNode2 = new SceneObjectNode();
    DrawModel* drawSphere2 = new DrawModel(mModel2);
    mNode2->AddDrawInterface(drawSphere2);
    mNode2->SetLocalPosition(glm::vec3(2.0f,0.0f,-10.0f));
    mRootNode->AddChild(mNode2);
    physx::PxTransform transform2(-2.1f,2.5f,-10.0f);
    physx::PxSphereGeometry geosphere2 = physx::PxSphereGeometry(1.0f);
    physx::PxRigidDynamic* spherebody2 = mPhysics.GetPhysics()->createRigidDynamic(transform2);
    spherebody2->attachShape(*mPhysics.GetPhysics()->createShape(geosphere2, *physxmat));
    physx::PxRigidBodyExt::updateMassAndInertia(*spherebody2, 10.0f);
    mNode2->SetBounds(spherebody2);
    mPhysics.AddPhysicsObject("sphere", mNode2);


    ModelMesh<SimpleVertex>* spheremesh3 = Primitives::MakeBox(10.0f, 0.25f, 50.0f);
    spheremesh3->SetShader(mEngine->GetShader());
    Material* material3 = new Material(mEngine->GetShader()->GetProgramID());
    material3->SetDiffuse(1,1,1);
    material3->SetAmbient(.1,.1,.1);
    material3->SetSpecular(0,1,0);
    material3->SetShininess(50.0f);
    spheremesh3->AddAsset(material3);
    mModel3 = new Model("box1");
    mModel3->AddMesh(spheremesh3);
    mNode3 = new SceneObjectNode();
    DrawModel* drawSphere3 = new DrawModel(mModel3);
    glm::mat4 offset = glm::mat4(1);
    offset = glm::translate(offset, glm::vec3(0,-.5f,0));
    drawSphere3->SetOffset(offset);
    mNode3->AddDrawInterface(drawSphere3);
    mNode3->SetLocalPosition(glm::vec3(-5.0f,-5.0f,-25.0f));
    mRootNode->AddChild(mNode3);
    physx::PxTransform transform3(0.0f,-5.0f,-25.0f);
    physx::PxBoxGeometry geobox1 = physx::PxBoxGeometry(10.0f/2, 0.25f/2, 50.0f/2);
    physx::PxRigidStatic* boxbody1 = mPhysics.GetPhysics()->createRigidStatic(transform3);
    boxbody1->attachShape(*mPhysics.GetPhysics()->createShape(geobox1, *physxmat));
    mNode3->SetBounds(boxbody1);
    mPhysics.AddPhysicsObject("groundplane", mNode3);



    for(int i = 0; i < 255; i++)
        mKeysDown[i] = 0;


    glClearColor(0.0, 0.0f, 0.0f, 1.0f); // Clear the background of our window to white
}

DebugState::~DebugState()
{

}

void DebugState::Update()
{
    std::cout << mEngine->GetFPS() << std::endl;
    std::cout.flush();

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
    int randomize = 1;//rand()%2;
    float x = ((float)rand() / 2147483647.0f) * 10.0f - 5.0f;
    float y = ((float)rand() / 2147483647.0f) * 10.0f;
    float z = ((float)rand() / 2147483647.0f) * 10.0f - 15;
    float r = ((float)rand() / 2147483647.0f) * 1.0f;
    float g = ((float)rand() / 2147483647.0f) * 1.0f;
    float b = ((float)rand() / 2147483647.0f) * 1.0f;

    Engine3d::SceneObjectNode* node = new Engine3d::SceneObjectNode();
    Engine3d::Material* material = new Engine3d::Material(mEngine->GetShader()->GetProgramID());
    material->SetDiffuse(r,g,b);
    physx::PxMaterial* physxmat = mPhysics.GetPhysics()->createMaterial(0.5f,0.5f,0.5f);



    if(randomize)
    {
        std::cout << "X " << x << " Y " << y << " Z " << z << std::endl;
        float l = ((float)rand()) / 2147483647.0f * 2.0f;
        float w = ((float)rand()) / 2147483647.0f * 2.0f;
        float h = ((float)rand()) / 2147483647.0f * 2.0f;
        Engine3d::ModelMesh<Engine3d::SimpleVertex>* mesh = Engine3d::Primitives::MakeBox(l,w,h);
        mesh->SetShader(mEngine->GetShader());
        mesh->AddAsset(material);
        Model* model = new Model("box");
        model->AddMesh(mesh);
        DrawModel* drawmodel = new DrawModel(model);
        glm::mat4 offset = glm::mat4(1);
        offset = glm::translate(offset, glm::vec3(l/2,w/2,h/2));
        drawmodel->SetOffset(offset);
        node->AddDrawInterface(drawmodel);
        node->Translate(glm::vec3(x,y,z));
        mRootNode->AddChild(node);
        physx::PxTransform transform1(x,y,z);
        physx::PxBoxGeometry geobox = physx::PxBoxGeometry(l,w,h);
        physx::PxRigidDynamic* boxbody = mPhysics.GetPhysics()->createRigidDynamic(transform1);
        boxbody->attachShape(*mPhysics.GetPhysics()->createShape(geobox, *physxmat));
        physx::PxRigidBodyExt::updateMassAndInertia(*boxbody, 1.0f);
        node->SetBounds(boxbody);
        char buffer[50];
        sprintf(buffer, "box%d", count);
        std::cout << buffer << std::endl;
        mPhysics.AddPhysicsObject(buffer, node);

    }
    else
    {

    }

    count++;
}
