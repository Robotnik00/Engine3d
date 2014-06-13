#pragma once

#include "Engine.h"
#define NDEBUG
#include <PxPhysicsAPI.h>

namespace Engine3d
{
    //////////////////////////////////////////////////////////////////
    /// This is where all states will be defined.
    ///
    //////////////////////////////////////////////////////////////////
    class Engine;

    // A state that the engine can be in.
    class State
    {
    public:
        State(Engine* engine)
        {
            mEngine = engine;
        }
        // synchronous callback to Engine
        virtual void Update() = 0;
        // asynchronous callback to Engine
        virtual void Draw(float delta) = 0;
        // asynchronous callback to Engine
        virtual void ProcessEvent(SDL_Event* event) = 0;


    protected:

        Engine* mEngine;

    };



}
