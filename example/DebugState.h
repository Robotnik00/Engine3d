#pragma once
#include <States.h>
#include <Scene.h>
#include <Model.h>

/// a simple debug state
class DebugState : public Engine3d::State
{
public:
    DebugState(Engine3d::Engine* engine);
    ~DebugState();
    void Update();
    void Draw(float delta);
    void ProcessEvent(SDL_Event* event);

protected:

    long mCounter;

    Engine3d::Model* mModel1;
    Engine3d::Model* mModel2;
    Engine3d::ModelMeshBase* mSphere;
    Engine3d::SceneObjectNode* mNode1;

    glm::mat4 mTransform;
    glm::mat4 mScale;
    bool mKeysDown[256];

};
