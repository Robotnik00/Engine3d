#include <PhysicsEngine.h>

using namespace Engine3d;

physx::PxDefaultErrorCallback PhysicsEngine::gDefaultErrorCallback;
physx::PxDefaultAllocator PhysicsEngine::gDefaultAllocatorCallback;

PhysicsEngine::PhysicsEngine()
{

}

bool PhysicsEngine::Initialize()
{
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if(!mFoundation)
    {
        return false;
    }

    mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
    if(!mProfileZoneManager)
    {
        return false;
    }

    bool recordMemoryAllocations = true;
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
                physx::PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager );

    if(!mPhysics)
    {
        return false;
    }

    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(3);
    if(!mCpuDispatcher)
    {
        return false;
    }

    sceneDesc.cpuDispatcher = mCpuDispatcher;
    sceneDesc.filterShader  = physx::PxDefaultSimulationFilterShader;


    mScene = mPhysics->createScene(sceneDesc);
    if(!mScene)
    {
        return false;
    }

    return true;
}

void PhysicsEngine::AddPhysicsObject(std::string name, SceneObjectNode *node)
{
    node->GetBounds()->setName(name.data());
    mScene->addActor(*node->GetBounds());

    mPhysicsObjects[name] = node;


}

void PhysicsEngine::Update(float dt)
{
    physx::PxActorTypeFlags desiredTypes = physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC;
    physx::PxU32 count = mScene->getNbActors(desiredTypes);
    physx::PxActor** buffer = new physx::PxActor*[count];
    mScene->getActors(desiredTypes, buffer, count);

    for(physx::PxU32 i = 0; i < count; i++)
    {
        if(buffer[i]->getOwnerClient() == physx::PX_DEFAULT_CLIENT)
        {
            physx::PxRigidActor* actor = buffer[i]->isRigidDynamic();
            if(actor != NULL)
            {
                physx::PxMat44 mat(actor->getGlobalPose());
                mPhysicsObjects[actor->getName()]->SetLocalTransform(*((glm::mat4*)&mat));
            }
        }
    }





    mScene->simulate(dt);
    mScene->fetchResults(true);
}
