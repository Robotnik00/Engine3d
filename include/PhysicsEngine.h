#pragma once

#include <Scene.h>

#include <map>
#include <string.h>

namespace Engine3d
{
    class PhysicsEngine
    {
    public:
        PhysicsEngine();
        bool Initialize();
        void Update(float dt);

        void AddPhysicsObject(std::string name, SceneObjectNode* node);

        physx::PxPhysics* GetPhysics() { return mPhysics; }
        physx::PxScene* GetScene() { return mScene; }

    protected:

        physx::PxFoundation* mFoundation;
        physx::PxProfileZoneManager* mProfileZoneManager;
        physx::PxPhysics* mPhysics;
        physx::PxScene* mScene;
        physx::PxCpuDispatcher* mCpuDispatcher;

        static physx::PxDefaultErrorCallback gDefaultErrorCallback;
        static physx::PxDefaultAllocator gDefaultAllocatorCallback;

        std::map<std::string, SceneObjectNode*> mPhysicsObjects;
    };
}
