//FileName:		viewcontroller.h
//Programmer:	Dan Cliburn, Cody Balos
//Date:			11/15/2016
//Purpose:		This file defines the header for the Viewcontroller class
//This class is responsible for setting up SDL and handing user input.

#pragma once

#include <SDL.h>  //Include SDL.h before glm.hpp
#include <glm.hpp>
#include "ViewModel.h"

using namespace glm;

class Viewcontroller
{
	enum Axis { X, Y, Z };

public:
	Viewcontroller();

	bool Init(game::Levels level);
	void HomeScreen();
	void display();
	void Run(game::Levels level);
	bool handleEvents(SDL_Event *theEvent);
	void updateLookAt();
	void LookDownAxis(Axis axis);
	void ResetView();

private:
	// state
	bool quit;

	//Variables to control user movement
	vec3 displacement; // camera displacement = glman displacement
	vec3 eye;
	vec3 aim;
	vec3 up;
	mat4 view_matrix;
	double MOVEANGLE;
	double LOOKANGLE;
	double moveForward;
	double moveSideways;
	double baseX;
	double baseY;

	// main model
	ViewModel theWorld;

	// sdl stuff
	SDL_Window *window;
	SDL_GLContext ogl4context;
};