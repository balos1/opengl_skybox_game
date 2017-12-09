/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#include "../Renderable.hpp"
#include "HallwayModel.hpp"
#include "../Transformation.h"

class StageModel : Renderable
{
public:
	StageModel();
	bool Init(std::shared_ptr<ShaderProgram>& shader) override;
	void Draw(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) override;

private:
	// properties
	Transformation matrices;
	std::vector<HallwayModel> hallways;
	std::vector<HallwayModel> elbows;
	std::vector<HallwayModel> t3s;
	std::vector<HallwayModel> t4s;

	glm::vec3 hallway_scale;
	glm::vec3 elbow_scale;
	glm::vec3 t3_scale;
	glm::vec3 t4_scale;
};
