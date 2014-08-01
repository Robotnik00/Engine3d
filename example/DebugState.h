



#pragma once


#include <States.h>
#include <Scene.h>
#include <Model.h>
#include <Input.h>

//#define NDEBUG
//#include <PxPhysicsAPI.h>

#include <PhysicsEngine.h>


/// a simple debug state
class DebugState : public Engine3d::State
{
public:
    DebugState(Engine3d::Engine* engine);
    ~DebugState();
    void Update();
    void Draw(float delta);
    void ProcessEvent(SDL_Event* event);

    void CreateRandomObject();

protected:

    long mCounter;

    Engine3d::SceneObjectNode* mRootNode;
    Engine3d::SceneObjectNode* mGimble;

    /*physx::PxMaterial* mMaterial1;
    physx::PxMaterial* mMaterial2;

    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    static physx::PxDefaultAllocator gDefaultAllocatorCallback;

    physx::PxFoundation* mFoundation;
    physx::PxProfileZoneManager* mProfileZoneManager;
    physx::PxPhysics* mPhysics;


    physx::PxScene* mScene;
    physx::PxCpuDispatcher* mCpuDispatcher;*/

    Engine3d::PhysicsEngine mPhysics;

    int  count, timestep;

    bool mKeysDown[256];

    Engine3d::UniformMat4* mCameraTransform;

};
