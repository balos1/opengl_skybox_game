/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#if _DEBUG
#define __DEBUG__ true
#else
#define __DEBUG__ false
#endif

#define SHADER_PATH std::string("..src/shaders")  // path to shader folder relative to exe
#define RES_PATH    std::string("..src/res")      // path to res folder relative to exe

#define WINDOWWIDTH  1600
#define WINDOWHEIGHT 800

// movement settings
#define ONE_UNIT     4.0f
#define TILE_SIZE    ONE_UNIT

// player settings
#define PLAYER_START    glm::vec3(ONE_UNIT/2, ONE_UNIT/2, ONE_UNIT/2)
#define PLAYER_BOX_SIZE 1.0f

// hallway settings
#define HALL_LENGTH  10.0f
#define HALL_WIDTH   3.5f
#define HALL_HEIGHT  2.5f


// energizer settings
#define ENERGIZER_SCALE glm::vec3(0.05f)


// game wide enums and stuff
namespace game
{
	enum Levels { GALAXY, DAYTIME, CITY };
}

