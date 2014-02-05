#pragma once

#include "Engine.h"
#include "Model.h"

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

/// a simple debug state
class DebugState : public State
{
public:
	DebugState(Engine* engine);
	~DebugState();
	void Update();
	void Draw(float delta);
	void ProcessEvent(SDL_Event* event);
	
protected:

	long mCounter;

	Model* mModel1;
	Model* mModel2;

	glm::mat4 mTransform;

	bool mKeysDown[256];

};


