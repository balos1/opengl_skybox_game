//FileName:		main.cpp
//Programmer:	Dan Cliburn, Cody Balos
//Date:			11/20/2016
//Purpose:		This file defines the main() function for Lab 15.
//Movement is controller with wasd and view direction is changed with the mouse.
//The program ends when the user presses the <Esc> key

#include "viewcontroller.h"


// Assignment Requirement Checklist:
// 1. Way more than 50 polygons
// 2. Users can move around virtual environment with keybaord/mouse
// 3. Three skyboxes, each being a different texture. 
// 4. Phong model implemented with 4 lights.
// 5. 4 shdader programs in use.
//
// EXTRA CREDIT:
//
//	I have a game with a clear win/loss state. You lose if you run out of time
//  before getting all the 'energizers'. 
// 
//  Oh and I spent ~60 hours on the project.
int main(int argc, char *argv[])  //main() must take these parameters when using SDL
{
	Viewcontroller vc;

	vc.HomeScreen();

	system("pause");

	return 0;
}