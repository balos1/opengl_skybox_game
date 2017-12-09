/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include "ScoreManager.hpp"

ScoreManager::ScoreManager()
	: score{0}
{
}

void ScoreManager::IncrementScore()
{
	++score;
}

int ScoreManager::GetScore() const
{
	return score;
}
