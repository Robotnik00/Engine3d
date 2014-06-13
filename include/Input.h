#pragma once

#include <Engine.h>

namespace Engine3d
{
    class EventListener
    {
    public:
        virtual void ProcessEvent(SDL_Event* event) = 0;
    };

    class KeyListener : public EventListener
    {
    public:
        bool KeyDown(char key) { return mKeysPressed[key]; }
        virtual void ProcessEvent(SDL_Event* event) {}
        bool mKeysPressed[255];
    };

}
