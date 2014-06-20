#pragma once

#include <Scene.h>

#include <map>
#include <string.h>

namespace Engine3d
{
    class PhysicsEngine;

    class PhysicsCallback
    {
    public:
        PhysicsCallback(std::string name, SceneObjectNode* node, ModelMeshBase* mesh);
        void SetIsStatic(bool t) { mIsStatic = t; }
        void Initialize(PhysicsEngine* physics);
        void UpdateTransform(glm::mat4 transform) { mNode->SetLocalTransform(transform); }



    protected:
        bool mIsStatic;
        std::string mName;
        SceneObjectNode* mNode;
        ModelMeshBase* mMesh;
        physx::PxMaterial* mPhysicsMat;
    };

    class PhysicsEngine
    {
    public:
        PhysicsEngine();
        bool Initialize();
        void Update(float dt);

        void AddPhysicsObject(std::string name, PhysicsCallback* node);

        physx::PxPhysics* GetPhysics() { return mPhysics; }
        physx::PxScene* GetScene() { return mScene; }
        physx::PxCooking* GetCooking() { return mCooking; }

        static void CalculateGeometry(ModelMeshBase* mesh);
    protected:

        physx::PxFoundation* mFoundation;
        physx::PxProfileZoneManager* mProfileZoneManager;
        physx::PxPhysics* mPhysics;
        physx::PxScene* mScene;
        physx::PxCpuDispatcher* mCpuDispatcher;
        physx::PxCooking* mCooking;

        static physx::PxDefaultErrorCallback gDefaultErrorCallback;
        static physx::PxDefaultAllocator gDefaultAllocatorCallback;

        std::map<std::string, PhysicsCallback*> mPhysicsObjects;
    };

}
