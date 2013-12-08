#include "Engine.h"

#include <GL/gl.h>

#include <iostream>


//////////////////////////////////////////////////////////////////
/// Methods for Engine:
///	
//////////////////////////////////////////////////////////////////
Engine::Engine(int width, int height)
{
	// protected
	mWidth			= width;
	mHeight			= height;
	mTime			= 0;
	mFPS			= 0.0f;
	mUpdateFrequency	= 25;
	mIsRunning		= false;
	mCurrentState 		= NULL;

	// private
	mUpdateFrames		= 0;
	mDrawFrames		= 0;

	// initialize video
	SDL_Init(SDL_INIT_EVERYTHING);
	mWindow = SDL_CreateWindow("",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	
	glViewport(0, 0, (GLsizei)GetWidth(), (GLsizei)GetHeight());
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(60, (GLfloat)GetWidth() / (GLfloat)GetHeight(), 1.0, 100.0); 
	glMatrixMode(GL_MODELVIEW);
	
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glViewport(0, 0, (GLsizei)GetWidth(), (GLsizei)GetHeight());
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(60, (GLfloat)GetWidth() / (GLfloat)GetHeight(), 1.0, 100.0); 
	glMatrixMode(GL_MODELVIEW);
	
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	GLchar* VertexShaderSource;
	GLchar* FragmentShaderSource;

	GLint vlength = LoadShader("src/VertexShader.cpp", &VertexShaderSource);
	GLint flength = LoadShader("src/FragmentShader.cpp", &FragmentShaderSource);


	GLuint vertexShader, fragmentShader;
	GLuint vertexShaderObject, fragmentShaderObject;	


	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSourceARB(vertexShaderObject, 1, (const GLchar**)&VertexShaderSource, &vlength);
	glShaderSourceARB(fragmentShaderObject, 1, (const GLchar**)&FragmentShaderSource, &flength);

	mProgramID = BuildShaders(vertexShaderObject, fragmentShaderObject);
	glUseProgram(mProgramID);
}
Engine::~Engine()
{
	// free up recources
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	
}

void Engine::Start()
{
	mIsRunning = true;
	Loop();
}
void Engine::ChangeState(State* state)
{
	if(mCurrentState != NULL)
	{
		delete mCurrentState;
	}
	mCurrentState = state;
}

void Engine::Loop()
{
	float  delta			= 0;    // percent to next update
	long   lastUpdate		= 0;    // time of last update
	long   lastDrawFrameCount	= 0;    // used to calculate fps	
	int    filterLength		= 30;   // length of moving avg
	float  first			= 0.0f; // first data point in filter
	float  last			= 0.0f; // last datapoint in filter
	float* FPSFilter		= new float[filterLength]; // used to average fps

	for(int i = 0; i < filterLength; i++)
		FPSFilter[i] = 0;

	

	while(mIsRunning)
	{	
		mTime = SDL_GetTicks();

		// if not done updating in time move to next update without drawing
		while(mTime - lastUpdate > 1000 / mUpdateFrequency)
		{
			mUpdateFrames++;
			
			// calculate fps
			first = FPSFilter[(mUpdateFrames + filterLength - 1) % (filterLength)];	
			last = FPSFilter[mUpdateFrames % (filterLength)];
			mFPS += (float)(first - last)/filterLength;
			FPSFilter[((mUpdateFrames)) % (filterLength)] = (float)(1000 * (mDrawFrames - lastDrawFrameCount)) / mUpdateFrequency;

			// process all events
			ProcessEvents();

			// update state
			if(mCurrentState != NULL)
			{
				mCurrentState->Update();
			}

			lastUpdate = mTime;
			lastDrawFrameCount = mDrawFrames;
		}

		
		
			
		// draw state	
		if(mCurrentState != NULL)
		{
			delta = ((float)(SDL_GetTicks() - lastUpdate) * mUpdateFrequency) / 1000.0f;
			mCurrentState->Draw(delta);
		}
		
		Display();
		mDrawFrames++;
	}
	delete FPSFilter;
}

void Engine::ProcessEvents()
{
	while(SDL_PollEvent(&mEvent))
	{
		if(mCurrentState != NULL)
		{
			mCurrentState->ProcessEvent(&mEvent);
		}

		switch(mEvent.type)
		{
		case SDL_KEYDOWN:
			if(mEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				mIsRunning = false;
			}
			break;
		case SDL_KEYUP:
			break;
	
		default:
			break;
		}
	}
}


void Engine::Display()
{		   
        SDL_RenderPresent(mRenderer);	
}


void Engine::SetTitle(const char* name)
{
	SDL_SetWindowTitle(mWindow, name);
}





