
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

using namespace Engine3d;

//////////////////////////////////////////////////////////////////
/// methods for DebugState:
///
//////////////////////////////////////////////////////////////////
DebugState::DebugState(Engine* engine)
    : State(engine)
{
    mModel1 = ModelLoader::Load("models/Armadillo/armadillo.3DS", mEngine->GetShader());
    mModel2 = ModelLoader::Load("models/Armadillo/armadillo.3DS", mEngine->GetShader());
    std::cout.flush();

    Texture* tex = new Texture("models/Armadillo/armadillotex.bmp");
    tex->Load();

    Texture* black = new Texture("models/Armadillo/black.bmp");
    black->Load();

    mModel1->GetMesh(2)->AddAsset(tex);
    mModel2->GetMesh(2)->AddAsset(tex);

    Uniform4f* light = new Uniform4f("light", engine->GetShader()->GetProgramID());
    light->SetVal(10,0,0,0);

    Uniform4f* color1 = new Uniform4f("color", engine->GetShader()->GetProgramID());
    color1->SetVal(.5,0,0,0);
    Uniform4f* color2 = new Uniform4f("color", engine->GetShader()->GetProgramID());
    color2->SetVal(0,.5,0,0);

    mModel1->GetMesh(2)->AddAsset(light);
    mModel2->GetMesh(2)->AddAsset(light);

    //mModel1->GetMesh(2)->AddAsset(color1);
    //mModel2->GetMesh(2)->AddAsset(color2);

    mTransform = glm::translate(mTransform, glm::vec3(0,0,-5));


    for(int i = 0; i < 255; i++)
        mKeysDown[i] = 0;



}

DebugState::~DebugState()
{

}

void DebugState::Update()
{



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
        mTransform = glm::rotate(mTransform, 10.0f, glm::vec3(1.0f,0,0));
    }
    if(mKeysDown['k'])
    {
        mTransform = glm::rotate(mTransform, 10.0f, glm::vec3(0,1.0f,0));
    }
    if(mKeysDown['l'])
    {
        mTransform = glm::rotate(mTransform, 10.0f, glm::vec3(0,0,1.0f));
    }
    if(mKeysDown['u'])
    {
        mTransform = glm::rotate(mTransform, -10.0f, glm::vec3(1.0f,0,0));
    }
    if(mKeysDown['i'])
    {
        mTransform = glm::rotate(mTransform, -10.0f, glm::vec3(0,1.0f,0));
    }
    if(mKeysDown['o'])
    {
        mTransform = glm::rotate(mTransform, -10.0f, glm::vec3(0,0,1.0f));
    }

    if(mKeysDown['w'])
    {
        mTransform = glm::translate(mTransform, glm::vec3(0,0,-0.05f));
    }
    if(mKeysDown['s'])
    {
        mTransform = glm::translate(mTransform, glm::vec3(0,0,0.05f));
    }
    if(mKeysDown['a'])
    {
        mTransform = glm::translate(mTransform, glm::vec3(-0.05f,0,0));
    }
    if(mKeysDown['d'])
    {
        mTransform = glm::translate(mTransform, glm::vec3(0.05f,0,0));
    }
    if(mKeysDown['z'])
    {
        mTransform = glm::translate(mTransform, glm::vec3(0,0.05f,0));
    }
    if(mKeysDown['x'])
    {
        mTransform = glm::translate(mTransform, glm::vec3(0,-0.05f,0));
    }









}

void DebugState::Draw(float delta)
{
    glClearColor(0.208f, 0.9f, 0.8f, 1.0f); // Clear the background of our window to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint myUniformLocation = glGetUniformLocation(mEngine->GetShader()->GetProgramID(), "light");
    glUniform4f(myUniformLocation, 5, 0, 0, .5);

    mModel1->Draw(&mTransform);
    glm::mat4 tmp = glm::translate(mTransform, glm::vec3(0,0,-0.5f));
    //mModel2->Draw(&tmp);
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
