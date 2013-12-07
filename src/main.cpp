#include "Engine.h"
#include "States.h"

int main(void)
{
	Engine* engine = new Engine(500, 500);
	State* state = new DebugState(engine);
	engine->ChangeState(state);
	engine->Start();
 	delete engine;

	return 0;
}

