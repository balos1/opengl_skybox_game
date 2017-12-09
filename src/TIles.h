/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#include <glew.h>
#include <glm.hpp>
#include <memory>
#include <vector>
#include "game_objects/GameObject.h"
#include "game_objects/EnergizerModel.hpp"


struct Tile
{
	GLfloat sideLength;
	glm::vec3 center;
	std::shared_ptr<EnergizerModel> energizer;   // energizer ?
	std::shared_ptr<GameObject> visitor; // glman or ghost
};

class Tiles
{
public:
	Tiles(GLfloat sideLength);
	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;

};
