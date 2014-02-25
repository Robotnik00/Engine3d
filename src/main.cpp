#include "Engine.h"
#include "States.h"

#include <iostream>

void callback()
{
	std::cout << "callback\n";
}

int main(void)
{
	
    Engine* engine = new Engine(500, 500);
    engine->SetInterval(callback, 1000);
    engine->RemoveInterval(callback);
    State* state = new DebugState(engine);
    engine->ChangeState(state);
    engine->Start();
    delete engine;

	return 0;
}

