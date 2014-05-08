#pragma once

#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES

#include <States.h>
#include <Shaders.h>


namespace Engine3d
{
    typedef void (*callbackptr)();

    class State;
    class Callback
    {
    public:
        Callback(void (*callback)(void), int delay);
        void Schedule(long ticks);
        callbackptr GetCallback();
    protected:
        void (*mCallback)(void) ;
        long mDelay;
        long mLastCall;
    };
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

        Shader* GetShader()		{ return mShader;		}

        void AddCallback(Callback* callback) { mCallbacks.push_back(callback); }
        void RemoveCallback(Callback* callback);
        void SetInterval(void (*function)(void), int delay);
        void RemoveInterval(void (*function)(void));

    protected:

        int   mWidth;
        int   mHeight;

        long  mTime;			// time an update started
        float mFPS;			// drawing frequency
        float mMaxFPS;
        int   mUpdateFrequency;		// frequency of update callback. must be less than 1000.

        bool  mIsRunning;

        State*		mCurrentState;	// the state the engine is in.
        SDL_Window*	mWindow;
        SDL_Renderer*	mRenderer;
        Shader* 	mShader;
        std::vector<Callback*> mCallbacks;
    private:

        long   mUpdateFrames;		// total number of updates
        long   mDrawFrames;		// total number of draws

        long   mTicks;

        SDL_Event mEvent;

    };

}
