

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
    mEngine->SetUpdateFrequency(100.0f);
    count = 0;
    if(mPhysics.Initialize())
    {
        std::cout << "physics initialized\n";
    }


    std::cout << sizeof(physx::PxVec3) << std::endl;

    mRootNode = new SceneObjectNode();
    Engine3d::Light* light = new Light(mEngine->GetShader()->GetProgramID());
    light->SetPosition(1.0,0,0);
    light->SetDiffuse(3,3,3);
    light->SetSpecular(1,1,1);
    light->SetAmbient(.1f,.1f,.1f);
    mRootNode->AddAsset(light);
    mRootNode->Translate(glm::vec3(-2.0f,-5.0f,7.0f));
    mGimble = new SceneObjectNode();
    mRootNode->AddChild(mGimble);
    mGimble->Rotate(15, glm::vec3(1,0,0));


    SceneObjectNode* node = new SceneObjectNode();
    ModelMesh<SimpleVertex>* mesh = Primitives::MakeBox(40,1,40);
    mesh->SetShader(mEngine->GetShader());
    Material* mat = new Material(mEngine->GetShader()->GetProgramID());
    mesh->AddAsset(mat);
    Model* model = new Model("plane");
    model->AddMesh(mesh);
    DrawModel* drawmodel = new DrawModel(model);
    node->AddDrawInterface(drawmodel);
    mGimble->AddChild(node);
    node->SetLocalPosition(glm::vec3(2.1f,0,-18));

    PhysicsCallback* physicscbplane = new PhysicsCallback("plane", node, mesh);
    physicscbplane->SetIsStatic(true);
    physicscbplane->Initialize(&mPhysics);




    Model* ship = new Model("models/Armadillo/armadillo.3DS");
    ModelLoader::Load(ship, mEngine->GetShader(), &mPhysics);
    Texture* texture = new Texture("models/Armadillo/armadillotex.bmp");
    texture->SetProgramID(mEngine->GetShader()->GetProgramID());
    texture->Load();
    ship->GetMesh(2)->AddAsset(mat);
    ship->GetMesh(2)->AddAsset(texture);
    SceneObjectNode* shipnode = new SceneObjectNode();
    DrawModel* drawship = new DrawModel(ship);
    shipnode->AddDrawInterface(drawship);
    mGimble->AddChild(shipnode);
    shipnode->Translate(glm::vec3(5,5,-20));
    PhysicsCallback* physicscbship1 = new PhysicsCallback("ship1", shipnode, ship->GetMesh(2));
    physicscbship1->Initialize(&mPhysics);


    Model* ship2 = new Model("models/Armadillo/armadillo.3DS");
    ModelLoader::Load(ship2, mEngine->GetShader(), &mPhysics);
    Texture* texture2 = new Texture("models/Armadillo/armadillotex.bmp");
    texture2->SetProgramID(mEngine->GetShader()->GetProgramID());
    texture2->Load();
    ship2->GetMesh(2)->AddAsset(mat);
    ship2->GetMesh(2)->AddAsset(texture2);
    SceneObjectNode* shipnode2 = new SceneObjectNode();
    DrawModel* drawship2 = new DrawModel(ship2);
    shipnode2->AddDrawInterface(drawship2);
    mGimble->AddChild(shipnode2);
    shipnode2->Translate(glm::vec3(3,5,-20));

    PhysicsCallback* physicscbship = new PhysicsCallback("ship2", shipnode2, ship2->GetMesh(2));
    physicscbship->Initialize(&mPhysics);

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
    if(timestep % 30 == 0)
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
    Engine3d::Material* mat1 = new Engine3d::Material(mEngine->GetShader()->GetProgramID());

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    mat1->SetDiffuse(r,g,b);

    int random = 1;//rand() % 2;

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
        mesh1->AddAsset(mat1);
        Engine3d::Model* model1 = new Model("box1");
        model1->AddMesh(mesh1);
        Engine3d::DrawInterface* drawmodel1 = new Engine3d::DrawModel(model1);
        node1->AddDrawInterface(drawmodel1);
        mGimble->AddChild(node1);
        node1->Translate(glm::vec3(x,y,z));
        char name[50];
        sprintf(name, "box%d", count);
        PhysicsCallback* physicscbbox = new PhysicsCallback(name, node1, mesh1);
        physicscbbox->Initialize(&mPhysics);
    }
    else
    {
        float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5;
        float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5 + 10;
        float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5 - 20;
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);



        physx::PxMaterial* physxmat = mPhysics.GetPhysics()->createMaterial(.1,.1,.1);

        SceneObjectNode* node1 = new SceneObjectNode();
        Engine3d::ModelMesh<Engine3d::SimpleVertex>* mesh1 = Engine3d::Primitives::MakeSphere(r,15,7);
        mesh1->SetShader(mEngine->GetShader());
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
        //node1->SetBounds(boxbody);
        char name[50];
        sprintf(name, "sphere%d", count);
        //mPhysics.AddPhysicsObject(name, node1);
    }
    count++;
    std::cout << count << std::endl;
}
