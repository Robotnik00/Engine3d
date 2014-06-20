#pragma once

#include <Model.h>
#include <Shaders.h>
#include <PhysicsEngine.h>

namespace Engine3d
{
    class ModelLoader
    {
    public:
        static void Load(Model* m, Shader* shader, PhysicsEngine* mPhysics = NULL);
    protected:
        static std::map<std::string, Model*> mLoadedModels;
    };
}
