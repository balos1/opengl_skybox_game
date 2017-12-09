//FileName:		Viewcontroller.cpp
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		This file defines the methods for the Viewcontroller class
//See:  http://www.sdltutorials.com/sdl-tutorial-basics/
//		http://www.sdltutorials.com/sdl-opengl-tutorial-basics/
//		http://stackoverflow.com/questions/13826150/sdl2-opengl3-how-to-initialize-sdl-inside-a-function
//for many more details on how to write an OpenGL program using SDL.  You might also want to go to these 
//pages which will link you to other tutorials on how to do stuff with SDL.
//Be warned, however, that a lot of the tutorials describe SDL 1.2, but we will be using SDL 2 in this course.
//
//Specific to this lab, I found some helpful information on the following pages for centering the mouse in SDL 2 
//and trapping it in the window (i.e. you can't move the mouse outside the window)
//	http://stackoverflow.com/questions/10492143/sdl2-mouse-grab-is-not-working
//	http://gamedev.stackexchange.com/questions/33519/trap-mouse-in-sdl
//	http://www.ginkgobitter.org/sdl/?SDL_ShowCursor
//
//A big change in this class is that the user now moves around the scene using a traditional
//first person controller. Movement is controlled with wasd and view direction is changed with the mouse.
//The program now ends when the user presses the <Esc> key.

#include <SDL.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "viewcontroller.h"
#include "config.inl"
#include <string>

using namespace glm;
using namespace std;

const double PI = M_PI;

Viewcontroller::Viewcontroller()
{
	quit        = false;
	window      = nullptr;
	ogl4context = nullptr;

	ResetView();
}

//Initializes SDL, GLEW, and OpenGL
bool Viewcontroller::Init(game::Levels level)
{
	//First initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Failed to initialize SDL." << endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	if ((window = SDL_CreateWindow("GL-Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGHT, SDL_WINDOW_OPENGL)) == nullptr)
	{
		cout << "Failed to create window." << endl;
		return false;
	}
	ogl4context = SDL_GL_CreateContext(window);
	SDL_ShowCursor(0);  //Hide the mouse cursor

	//Initialize the ViewModel that you want to render
	if (theWorld.Init(level) == false)  //OpenGL initialization is done in the ViewModel class
	{
		cout << "Failed to initialize theWorld." << endl;
		return false;
	}

	return true;  //Everything got initialized
}

void Viewcontroller::HomeScreen()
{
	cout << "Welcome to GL-man!" << endl;
	cout << "You are GL-Man, the beloved hero of COMP 153." << endl;
	cout << "Try and collect all of the special red energizers without getting caught by the clock!" << endl;
	cout << "Use the arrow/wsad keys and mouse to move around." << endl;
	cout << "Select a level to play." << endl;
	cout << "For GALAXY level                : Press 0" << endl;
	cout << "For DAYTIME level               : Press 1" << endl;
	cout << "For CITY level                  : Press 2" << endl;
	cout << "To Let Everyone Down (i.e. Quit): Press any other key." << endl;
	char resp = cin.get();
	if (resp == '0')
		Run(game::GALAXY);
	else if (resp == '1')
		Run(game::DAYTIME);
	else if (resp == '2')
		Run(game::CITY);
}

//Display what we want to see in the graphics window
void Viewcontroller::display()
{
	theWorld.Draw();

	SDL_GL_SwapWindow(window);
}

bool Viewcontroller::handleEvents(SDL_Event *theEvent)
{
	switch(theEvent->type)
	{
		case SDL_QUIT:  //User pressed the escape key
		{
			return true;  //force program to quit
		}
		case SDL_KEYDOWN:
		{
			if (theEvent->key.keysym.sym == SDLK_ESCAPE)  //the <Esc> key
			{
				return true;  //force game to quit
			}
			else if (theEvent->key.keysym.sym == SDLK_UP || theEvent->key.keysym.sym == SDLK_w)
			{
				moveForward = 0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_LEFT || theEvent->key.keysym.sym == SDLK_a)
			{
				moveSideways = -0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_RIGHT || theEvent->key.keysym.sym == SDLK_d)
			{
				moveSideways = 0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_DOWN || theEvent->key.keysym.sym == SDLK_s)
			{
				moveForward = -0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_r)
			{
				ResetView();
			}
			else if (theEvent->key.keysym.sym == SDLK_x)
			{
				LookDownAxis(X);
			}
			else if (theEvent->key.keysym.sym == SDLK_y)
			{
				LookDownAxis(Y);
			}
			else if (theEvent->key.keysym.sym == SDLK_z)
			{
				LookDownAxis(Z);
			}
			else if (theEvent->key.keysym.sym == SDLK_SPACE)
			{
				// NO-OP
			}
			break;
		}
		case SDL_KEYUP:
		{
			if (theEvent->key.keysym.sym == SDLK_UP || theEvent->key.keysym.sym == SDLK_DOWN || theEvent->key.keysym.sym == SDLK_w || theEvent->key.keysym.sym == SDLK_s)
			{
				moveForward = 0;
			}
			else if (theEvent->key.keysym.sym == SDLK_LEFT || theEvent->key.keysym.sym == SDLK_RIGHT || theEvent->key.keysym.sym == SDLK_a || theEvent->key.keysym.sym == SDLK_d)
			{
				moveSideways = 0;
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			const double MOUSE_SENSITIVITY_X = .01;
			const double MOUSE_SENSITIVITY_Y = .01;
			
			MOVEANGLE += (theEvent->button.x - baseX) * MOUSE_SENSITIVITY_X;
			LOOKANGLE += -(theEvent->button.y - baseY) * MOUSE_SENSITIVITY_Y;

			SDL_WarpMouseInWindow(window, baseX, baseY);  //re-center the mouse cursor
			break;
		}
	} //end the switch
	return false;  //the program should not end
}

void Viewcontroller::updateLookAt()
{
	glm::vec3 newEye {
		eye.x + cosf(MOVEANGLE)*moveForward + cosf(MOVEANGLE + M_PI / 2.0)*moveSideways,
		eye.y,
		eye.z + sinf(MOVEANGLE)*moveForward + sinf(MOVEANGLE + M_PI / 2.0)*moveSideways
	};

	// calculate a displacement vector
	displacement = newEye - eye;
	eye = newEye;

	//Adjust the aim position from the new eye position
	aim[0] = eye[0] + cosf(MOVEANGLE);
	aim[1] = eye[1] + LOOKANGLE;
	aim[2] = eye[2] + sinf(MOVEANGLE);

	view_matrix = lookAt(eye, aim, up);  //calculate the view orientation matrix
	theWorld.SetViewMatrix(view_matrix);
	theWorld.DoMovement(displacement);
}

void Viewcontroller::LookDownAxis(Axis axis)
{
	view_matrix = mat4(1.0);
	moveForward = 0.0;
	moveSideways = 0.0;
	MOVEANGLE = PI / 2.0;
	LOOKANGLE = 0.0;

	if (axis == X)
		aim = vec3(eye.x + 10.0f, eye.y, eye.z);
	else if (axis == Y)
		aim = vec3(eye.x, eye.y + 10.0f, eye.z);
	else if (axis == Z)
		aim = vec3(eye.x, eye.y, eye.z + 10.0f);

	view_matrix = lookAt(eye, aim, up);  //calculate the view orientation matrix
	theWorld.SetViewMatrix(view_matrix);
}

void Viewcontroller::ResetView()
{
	view_matrix = mat4(1.0);
	moveForward = 0.0;
	moveSideways = 0.0;
	MOVEANGLE = PI / 2.0;
	LOOKANGLE = 0.0;
	baseX = WINDOWWIDTH / 2.0;
	baseY = WINDOWHEIGHT / 2.0;
	displacement = PLAYER_START - eye;
	
	eye = PLAYER_START;
	up = vec3(0.0, 1.0, 0.0);
	aim = vec3(0.0, PLAYER_START.y, 1.0f);

	view_matrix = lookAt(eye, aim, up);
	theWorld.SetViewMatrix(view_matrix);
	theWorld.DoMovement(displacement);
}

void Viewcontroller::Run(game::Levels level)
{
	if (Init(level) == false)  //This method (defined above) sets up OpenGL, SDL, and GLEW
	{
		cout << "Program failed to initialize ... exiting." << endl;
		return;
	}

	SDL_Event events;  //Makes an SDL_Events object that we can use to handle events

	const long MAX_TIME = 2 * 60 * 1000;
	const int UPDATE_FREQUENCY = 10; //update the frame every 10 milliseconds
	long currentTime, startTime = clock();
	long totalTime = 0;
	SDL_WarpMouseInWindow(window, baseX, baseY);  //Center the mouse cursor
	do
	{
		display();  //This method (defined above) draws whatever we have defined
		while (SDL_PollEvent(&events)) //keep processing the events as long as there are events to process
			quit = handleEvents(&events);

		currentTime = clock();
		if (currentTime - startTime > UPDATE_FREQUENCY)
		{
			updateLookAt();
			totalTime += (currentTime - startTime);
			if (totalTime > MAX_TIME)
				quit = true;
			quit = (totalTime > MAX_TIME) || quit || !theWorld.OnTick();
			startTime = currentTime;
		}

	} while (!quit); //run until "quit" is true (i.e. user presses the <Esc> key)

	SDL_GL_DeleteContext(ogl4context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	cout << "Your score was: "  << std::to_string(theWorld.score_manager->GetScore()) << endl;
	cout << " Your time was: "  << std::to_string(totalTime/1000) + " seconds" << endl;

	HomeScreen();
}
