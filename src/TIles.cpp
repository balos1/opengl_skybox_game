/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include "Tiles.h"

Tiles::Tiles(GLfloat sideLength)
{
	// For now the grid is hard coded to be tic-tac-toe style layout (with a border).
	// The vertical (not really vertical since its in the xz plane, but think 2D),
	// corridors are spaced 4 times the width of the corridor apart.
	// The same goes for the horizontals.
	const GLfloat middle = sideLength / 2.0f;
	const GLfloat height = sideLength / 2.1f;
	
	tiles.resize(8);
	for (auto i = 1; i < 9; ++i)
	{
		tiles.at(i-1).resize(8);
		for (auto j = 1; j < 9; ++j)
		{
			Tile tile{ sideLength, glm::vec3(i*2 + middle, height, j*2 + middle) };
			tiles.at(i-1).at(j-1) = std::make_unique<Tile>(tile);
		}
	}
}
