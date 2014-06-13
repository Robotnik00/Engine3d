#pragma once

#include <Model.h>

namespace Engine3d {

    class Primitives
    {
    public:
        static ModelMesh<SimpleVertex>* MakeSphere(float r, int v, int h);
        static ModelMesh<SimpleVertex>* MakeBox(float l, float w, float h);
    };
}
