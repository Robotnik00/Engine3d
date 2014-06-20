#include <PhysicsEngine.h>
#include <iostream>

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
    //physx::PxCookingParams p = physx::PxCookingParams();
    mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(mPhysics->getTolerancesScale()));
    if (!mCooking)
    {
        return false;
    }
    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
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

void PhysicsEngine::AddPhysicsObject(std::string name, PhysicsCallback *p)
{
    mPhysicsObjects[name] = p;
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
                mPhysicsObjects[actor->getName()]->UpdateTransform(*((glm::mat4*)&mat));
                std::cout << "hey\n";
                std::cout.flush();
            }
        }
    }





    mScene->simulate(dt);
    mScene->fetchResults(true);
}

PhysicsCallback::PhysicsCallback(std::string name, SceneObjectNode *node, ModelMeshBase *mesh)
{
    mName = name;
    mNode = node;
    mMesh = mesh;
    mIsStatic = false;
}

void PhysicsCallback::Initialize(PhysicsEngine *physics)
{
    mPhysicsMat = physics->GetPhysics()->createMaterial(0.5f,0.5f,0.5f);
    physx::PxShape* shape = physics->GetPhysics()->createShape(*mMesh->GetBounds(), *mPhysicsMat);
    glm::mat4 tmp = mNode->GetLocalTransform();
    physx::PxRigidActor* actor;
    if(!mIsStatic)
    {
        actor = physics->GetPhysics()->createRigidDynamic(physx::PxTransform(*((physx::PxMat44*)&tmp)));
        actor->attachShape(*shape);
        physx::PxRigidBodyExt::updateMassAndInertia(*(physx::PxRigidDynamic*)actor, 100.0f);
    }
    else
    {
        actor = physics->GetPhysics()->createRigidStatic(physx::PxTransform(*((physx::PxMat44*)&tmp)));
        actor->attachShape(*shape);
    }

    actor->setName(mName.data());


    physics->AddPhysicsObject(mName, this);
    physics->GetScene()->addActor(*actor);

}

