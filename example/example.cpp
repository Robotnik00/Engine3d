#include <Engine.h>
#include <States.h>
#include <DebugState.h>
#include <iostream>

void callback()
{
    std::cout << "callback\n";
    std::cout.flush();
}

int main(void)
{
    Engine3d::Engine* engine = new Engine3d::Engine(1000, 1000);
    //engine->SetInterval(callback, 100);
    //engine->RemoveInterval(callback);
    Engine3d::State* state = new DebugState(engine);
    engine->ChangeState(state);
    engine->Start();
    delete engine;


	return 0;
}

