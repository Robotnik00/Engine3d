

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
    mEngine->SetUpdateFrequency(30.0f);
    count = 0;
    if(mPhysics.Initialize())
    {
        std::cout << "physics initialized\n";
    }


    std::cout << sizeof(physx::PxVec3) << std::endl;

    mRootNode = new SceneObjectNode();
    Engine3d::Light* light = new Light(mEngine->GetShader()->GetProgramID());
    light->SetPosition(2.1f,5.0f,-18.0f);
    light->SetDiffuse(10,10,10);
    light->SetSpecular(10,10,10);
    light->SetConstAttenuation(.01);
    light->SetAmbient(0.2f,0.2f,0.2f);
    mGimble = new SceneObjectNode();
    mRootNode->AddChild(mGimble);

    mGimble->AddAsset(light);

    CameraLocationUpdater* cam = new CameraLocationUpdater(mEngine->GetShader()->GetProgramID());
    mGimble->AddDrawInterface(cam);


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




//    Model* ship = new Model("models/Armadillo/armadillo.3DS");
//    ModelLoader::Load(ship, mEngine->GetShader(), &mPhysics);
//    Texture* texture = new Texture("models/Armadillo/armadillotex.bmp");
//    texture->SetProgramID(mEngine->GetShader()->GetProgramID());
//    texture->Load();
//    ship->GetMesh(2)->AddAsset(mat);
//    ship->GetMesh(2)->AddAsset(texture);
//    SceneObjectNode* shipnode = new SceneObjectNode();
//    DrawModel* drawship = new DrawModel(ship);
//    shipnode->AddDrawInterface(drawship);

//    mGimble->AddChild(shipnode);
//    shipnode->Translate(glm::vec3(5,5,-20));
//    PhysicsCallback* physicscbship1 = new PhysicsCallback("ship1", shipnode, ship->GetMesh(2));
//    physicscbship1->Initialize(&mPhysics);


//    Model* ship2 = new Model("models/Armadillo/armadillo.3DS");
//    ModelLoader::Load(ship2, mEngine->GetShader(), &mPhysics);
//    Texture* texture2 = new Texture("models/Armadillo/armadillotex.bmp");
//    texture2->SetProgramID(mEngine->GetShader()->GetProgramID());
//    texture2->Load();
//    ship2->GetMesh(2)->AddAsset(mat);
//    ship2->GetMesh(2)->AddAsset(texture2);
//    SceneObjectNode* shipnode2 = new SceneObjectNode();
//    DrawModel* drawship2 = new DrawModel(ship2);
//    shipnode2->AddDrawInterface(drawship2);
//    mGimble->AddChild(shipnode2);
//    shipnode2->Translate(glm::vec3(3,5,-20));
//    PhysicsCallback* physicscbship = new PhysicsCallback("ship2", shipnode2, ship2->GetMesh(2));
//    physicscbship->Initialize(&mPhysics);


//    CreateRandomObject();



    SceneObjectNode* node1 = new SceneObjectNode();

    ModelMesh<SimpleVertex>* box = Primitives::MakeBox(1,1,1);
    Material* mat1 = new Engine3d::Material(mEngine->GetShader()->GetProgramID());
    mat1->SetDiffuse(1.0f,0.0f,0.0f);
    mat1->SetShininess(200);
    box->AddAsset(mat1);
    box->SetShader(mEngine->GetShader());
    DrawMesh* drawmesh = new DrawMesh(box);
    node1->AddDrawInterface(drawmesh);
    node1->SetLocalPosition(glm::vec3(3.0f,20.0f,-20.0f));
    PhysicsCallback* phys = new PhysicsCallback("box", node1, box);
    phys->Initialize(&mPhysics);

    mGimble->AddChild(node1);


    for(int i = 0; i < 255; i++)
        mKeysDown[i] = 0;


    mCameraTransform = new UniformMat4("cameratransform", mEngine->GetShader()->GetProgramID());





    glClearColor(0.0, 1.0f, 1.0f, 1.0f); // Clear the background of our window to whitex
}

DebugState::~DebugState()
{

}

void DebugState::Update()
{


    char buffer[50];
    sprintf(buffer, "%2.2f", mEngine->GetFPS());
    mEngine->SetTitle(buffer);

    if(mKeysDown['w'] == 1)
    {
        glm::vec4 axis = glm::vec4(0,0,1,1);
        axis = glm::transpose(mRootNode->GetGlobalTransform()) * axis;
        axis = (glm::normalize(axis) * 0.6f);
        mGimble->Translate(glm::vec3(axis.x, axis.y, axis.z));
    }
    if(mKeysDown['a'] == 1)
    {
        glm::vec4 axis = glm::vec4(1,0,0,1);
        axis = glm::transpose(mRootNode->GetGlobalTransform()) * axis;
        axis = (glm::normalize(axis) * 0.6f);
        mGimble->Translate(glm::vec3(axis.x, axis.y, axis.z));
    }
    if(mKeysDown['s'] == 1)
    {
        glm::vec4 axis = glm::vec4(0,0,-1,1);
        axis = glm::transpose(mRootNode->GetGlobalTransform()) * axis;
        axis = (glm::normalize(axis) * 0.6f);
        mGimble->Translate(glm::vec3(axis.x, axis.y, axis.z));
    }
    if(mKeysDown['d'] == 1)
    {
        glm::vec4 axis = glm::vec4(-1,0,0,1);
        axis = glm::transpose(mRootNode->GetGlobalTransform()) * axis;
        axis = (glm::normalize(axis) * 0.6f);
        mGimble->Translate(glm::vec3(axis.x, axis.y, axis.z));
    }
    if(mKeysDown['x'] == 1)
    {
        glm::vec4 axis = glm::vec4(0,-1,0,1);
        axis = glm::transpose(mRootNode->GetGlobalTransform()) * axis;
        axis = (glm::normalize(axis) * 0.6f);
        mGimble->Translate(glm::vec3(axis.x, axis.y, axis.z));
    }
    if(mKeysDown['z'] == 1)
    {
        glm::vec4 axis = glm::vec4(0,1,0,1);
        axis = glm::transpose(mRootNode->GetGlobalTransform()) * axis;
        axis = (glm::normalize(axis) * 0.6f);
        mGimble->Translate(glm::vec3(axis.x, axis.y, axis.z));
    }
    if(mKeysDown['q'] == 1)
    {
        glm::vec4 axis = glm::vec4(0,0,1,1);
        mRootNode->Rotate(-3.0, glm::vec3(axis.x,axis.y,axis.z));
    }
    if(mKeysDown['e'] == 1)
    {
        glm::vec4 axis = glm::vec4(0,0,-1,1);
        mRootNode->Rotate(-3.0, glm::vec3(axis.x,axis.y,axis.z));
    }
    if(mKeysDown[80] == 1) // left
    {
        glm::vec4 axis = glm::vec4(0,1,0,1);
        mRootNode->Rotate(-3.0, glm::vec3(axis.x,axis.y,axis.z));
    }
    if(mKeysDown[82] == 1) // up
    {
        glm::vec4 axis = glm::vec4(1,0,0,1);
        mRootNode->Rotate(-3.0, glm::vec3(axis.x,axis.y,axis.z));
    }
    if(mKeysDown[79] == 1) // right
    {
        glm::vec4 axis = glm::vec4(0,-1,0,1);
        mRootNode->Rotate(-3.0, glm::vec3(axis.x,axis.y,axis.z));
    }
    if(mKeysDown[81] == 1) // down
    {
        glm::vec4 axis = glm::vec4(-1,0,0,1);
        mRootNode->Rotate(-3.0, glm::vec3(axis.x,axis.y,axis.z));
    }

    if(glm::length(mRotAxis) > 1.0 && mMouseDown == true)
    {
        mRootNode->Rotate(glm::length(mRotAxis), glm::normalize(mRotAxis));
    }

    timestep++;
    if(timestep % 10 == 0)
    {
        //CreateRandomObject();
    }

    mPhysics.Update(1.0f/mEngine->GetUpdateFrequency());
    mRootNode->Update();

}

void DebugState::Draw(float delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mRootNode->Draw(delta);

}

void DebugState::ProcessEvent(SDL_Event* event)
{
    switch(event->type)
    {
    case SDL_KEYDOWN:
        mKeysDown[event->key.keysym.sym%256] = true;
        break;
    case SDL_KEYUP:

        mKeysDown[event->key.keysym.sym%256] = false;

        break;
    case SDL_MOUSEMOTION:
    {
        mRotAxis = glm::vec3(event->motion.x - mEngine->GetWidth()/2,
                                      -1 * (event->motion.y - mEngine->GetHeight()/2), 0);
        mRotAxis = glm::cross(mRotAxis, glm::vec3(0,0,-1));

        mRotAxis.x = mRotAxis.x * 0.01;
        mRotAxis.y = mRotAxis.y * 0.01;
        mRotAxis.z = mRotAxis.z * 0.01;

        std::cout << mRotAxis.x << ' ' << mRotAxis.y << ' ' << mRotAxis.z << "   " << glm::length(mRotAxis) << "\n";
        std::cout.flush();

        break;
    }
    case SDL_MOUSEBUTTONDOWN:
        mMouseDown = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mMouseDown = false;
        break;


    default:
        break;
    }
}

void DebugState::CreateRandomObject()
{
    Engine3d::Material* mat1 = new Engine3d::Material(mEngine->GetShader()->GetProgramID());

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    mat1->SetDiffuse(r,g,b);
    mat1->SetSpecular(3,3,3);
    mat1->SetShininess(200);
    int random = rand();

    if(random % 2 == 0)
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
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)/2;



        physx::PxMaterial* physxmat = mPhysics.GetPhysics()->createMaterial(.1,.1,.1);

        SceneObjectNode* node1 = new SceneObjectNode();
        Engine3d::ModelMesh<Engine3d::SimpleVertex>* mesh1 = Engine3d::Primitives::MakeSphere(r,20,10);
        mesh1->SetShader(mEngine->GetShader());
        mesh1->AddAsset(mat1);
        Engine3d::Model* model1 = new Model("box1");
        model1->AddMesh(mesh1);
        Engine3d::DrawInterface* drawmodel1 = new Engine3d::DrawModel(model1);
        node1->AddDrawInterface(drawmodel1);
        node1->Translate(glm::vec3(x,y,z));
        mGimble->AddChild(node1);
        char name[50];
        sprintf(name, "sphere%d", count);
        PhysicsCallback* physicssphere = new PhysicsCallback(name, node1, mesh1);
        physicssphere->Initialize(&mPhysics);
    }
    count++;
    std::cout << count << std::endl;
}
