
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

//////////////////////////////////////////////////////////////////
/// methods for DebugState:
///
//////////////////////////////////////////////////////////////////
DebugState::DebugState(Engine* engine)
    : State(engine)
{
    mModel1 = ModelLoader::Load("models/Armadillo/armadillo.3DS", mEngine->GetShader());
    std::cout.flush();


    mSphere = Primitives::MakeSphere(.5,20,10);
    mSphere->SetShader(mEngine->GetShader());
    //mModel1->AddMesh(mSphere);

    Texture* tex = new Texture("models/Armadillo/armadillotex.bmp");
    tex->Load();


    Material* material = new Material(mEngine->GetShader()->GetProgramID());
    Light* light = new Light(mEngine->GetShader()->GetProgramID());

    mSphere->AddAsset(material);
    mSphere->AddAsset(light);

    mModel1->GetMesh(0)->AddAsset(material);
    mModel1->GetMesh(0)->AddAsset(light);
    mModel1->GetMesh(1)->AddAsset(material);
    mModel1->GetMesh(1)->AddAsset(light);
    mModel1->GetMesh(2)->AddAsset(material);
    mModel1->GetMesh(2)->AddAsset(light);
    mModel1->GetMesh(2)->AddAsset(tex);
    light->SetPosition(.5,0,-.5);
    light->SetDiffuse(5,5,5);

    mTransform = glm::translate(mTransform, glm::vec3(0,0,-5));

    mNode1 = new Engine3d::SceneObjectNode();
    mNode1->SetLocalPosition(glm::vec3(0,0,-5));
    Engine3d:;DrawModel* drawmodel = new Engine3d::DrawModel(mModel1);
    mNode1->AddDrawInterface(drawmodel);

    Engine3d::SceneObjectNode* node2 = new Engine3d::SceneObjectNode();
    node2->AddDrawInterface(drawmodel);
    node2->SetLocalPosition(glm::vec3(0,1,0));
    mNode1->AddChild(node2);



    Engine3d::SceneObjectNode* node3 = new Engine3d::SceneObjectNode();
    Engine3d::DrawMesh<SimpleVertex>* drawmesh = new Engine3d::DrawMesh<SimpleVertex>((Engine3d::ModelMesh<SimpleVertex>*)mSphere);
    node3->AddDrawInterface(drawmesh);
    node3->SetLocalPosition(glm::vec3(0,2,0));
    mNode1->AddChild(node3);


    for(int i = 0; i < 255; i++)
        mKeysDown[i] = 0;



}

DebugState::~DebugState()
{

}

void DebugState::Update()
{



    mNode1->Update();
    mCounter++;
    if(mCounter % 25 == 0)
    {
        char name[20];
        sprintf(name, "%.2f", mEngine->GetFPS());
        mEngine->SetTitle(name);
        //printf("update callback: %d Hz.	FPS: %.2f Hz.\n", mEngine->GetUpdateFrequency(), mEngine->GetFPS());
    }
    if(mKeysDown['j'])
    {
        mNode1->Rotate(10.0f, glm::vec3(1.0f,0,0));
    }
    if(mKeysDown['k'])
    {
        mNode1->Rotate(10.0f, glm::vec3(0,1.0f,0));
    }
    if(mKeysDown['l'])
    {
        mNode1->Rotate(10.0f, glm::vec3(0,0,1.0f));
    }
    if(mKeysDown['u'])
    {
        mNode1->Rotate(-10.0f, glm::vec3(1.0f,0,0));
    }
    if(mKeysDown['i'])
    {
        mNode1->Rotate(-10.0f, glm::vec3(0,1.0f,0));
    }
    if(mKeysDown['o'])
    {
        mNode1->Rotate(-10.0f, glm::vec3(0,0,1.0f));
    }

    if(mKeysDown['w'])
    {
        mNode1->Translate(glm::vec3(0,0,-0.05f));
    }
    if(mKeysDown['s'])
    {
        mNode1->Translate(glm::vec3(0,0,0.05f));
    }
    if(mKeysDown['a'])
    {
        mNode1->Translate(glm::vec3(-0.05,0,0));
    }
    if(mKeysDown['d'])
    {
        mNode1->Translate(glm::vec3(0.05,0,0));
    }
    if(mKeysDown['z'])
    {
        mNode1->Translate(glm::vec3(0,0.05f,0));
    }
    if(mKeysDown['x'])
    {
        mNode1->Translate(glm::vec3(0,-0.05f,0));
    }








}

void DebugState::Draw(float delta)
{
    glClearColor(0.208f, 0.9f, 0.8f, 1.0f); // Clear the background of our window to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mNode1->Draw(delta);

}
void DebugState::ProcessEvent(SDL_Event* event)
{
    switch(event->type)
    {
    case SDL_KEYDOWN:
        if(event->key.keysym.sym == SDLK_w)
        {
            mKeysDown['w'] = true;
        }
        else if(event->key.keysym.sym == SDLK_s)
        {
            mKeysDown['s'] = true;
        }
        else if(event->key.keysym.sym == SDLK_a)
        {
            mKeysDown['a'] = true;
        }
        else if(event->key.keysym.sym == SDLK_d)
        {
            mKeysDown['d'] = true;
        }
        else if(event->key.keysym.sym == SDLK_z)
        {
            mKeysDown['z'] = true;
        }
        else if(event->key.keysym.sym == SDLK_x)
        {
            mKeysDown['x'] = true;
        }
        else if(event->key.keysym.sym == SDLK_j)
        {
            mKeysDown['j'] = true;
        }
        else if(event->key.keysym.sym == SDLK_k)
        {
            mKeysDown['k'] = true;
        }
        else if(event->key.keysym.sym == SDLK_l)
        {
            mKeysDown['l'] = true;
        }
        else if(event->key.keysym.sym == SDLK_u)
        {
            mKeysDown['u'] = true;
        }
        else if(event->key.keysym.sym == SDLK_i)
        {
            mKeysDown['i'] = true;
        }
        else if(event->key.keysym.sym == SDLK_o)
        {
            mKeysDown['o'] = true;
        }

        break;
    case SDL_KEYUP:
        if(event->key.keysym.sym == SDLK_w)
        {
            mKeysDown['w'] = false;
        }
        else if(event->key.keysym.sym == SDLK_s)
        {
            mKeysDown['s'] = false;
        }
        else if(event->key.keysym.sym == SDLK_a)
        {
            mKeysDown['a'] = false;
        }
        else if(event->key.keysym.sym == SDLK_d)
        {
            mKeysDown['d'] = false;
        }
        else if(event->key.keysym.sym == SDLK_z)
        {
            mKeysDown['z'] = false;
        }
        else if(event->key.keysym.sym == SDLK_x)
        {
            mKeysDown['x'] = false;
        }
        else if(event->key.keysym.sym == SDLK_j)
        {
            mKeysDown['j'] = false;
        }
        else if(event->key.keysym.sym == SDLK_k)
        {
            mKeysDown['k'] = false;
        }
        else if(event->key.keysym.sym == SDLK_l)
        {
            mKeysDown['l'] = false;
        }
        else if(event->key.keysym.sym == SDLK_u)
        {
            mKeysDown['u'] = false;
        }
        else if(event->key.keysym.sym == SDLK_i)
        {
            mKeysDown['i'] = false;
        }
        else if(event->key.keysym.sym == SDLK_o)
        {
            mKeysDown['o'] = false;
        }

        break;

    default:
        break;
    }
}
