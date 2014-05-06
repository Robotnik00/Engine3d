#pragma once

#include <Model.h>
#include <Shaders.h>

namespace Engine3d
{
    class ModelLoader
    {
    public:
        static Model* Load(const std::string filename, Shader* shader);
    };
}
