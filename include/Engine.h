#pragma once

#include "SDL.h"
#include "States.h"

class State;

// creates a Loop that updates at a constant frequency
class Engine
{

public:

	Engine(int width, int height);
	~Engine();	

	// starts the game engine	
	void Start();

	// change to a new state
	void ChangeState(State* state);	

	// callback from Renderer
	void Loop();
	
	// draws window
	void Display();
	
	// process key events
	void ProcessEvents();

	void	SetTitle(const char* name);
	void	SetUpdateFrequency(int updateFrequency);

	int	GetWidth()		{ return mWidth;		}
	int	GetHeight()		{ return mHeight;		}	
	long	GetUpdateFrequency()	{ return mUpdateFrequency;	}
	float	GetFPS()		{ return mFPS;			}	
	long	GetTime()		{ return mTime;			}
	
	SDL_Renderer* GetRenderer()	{ return mRenderer;		}
	
	bool	isRunning()		{ return mIsRunning;		}

protected:
	

	int   mWidth;		
	int   mHeight;

	long  mTime;			// time an update started
	float mFPS;			// drawing frequency
	int   mUpdateFrequency;		// frequency of update callback. must be less than 1000.  
	
	bool  mIsRunning;
	
	State*		mCurrentState;	// the state the engine is in.
	SDL_Window*	mWindow;	
	SDL_Renderer*	mRenderer;

private:

	long   mUpdateFrames;		// total number of updates
	long   mDrawFrames;		// total number of draws
			

	SDL_Event mEvent;

};

