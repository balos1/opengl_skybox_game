/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#include "../meshes/SphereMesh.hpp"
#include "../AABB.hpp"
#include "GameObject.h"

class EnergizerModel : public GameObject
{
public:
	explicit EnergizerModel(glm::vec3 center, std::shared_ptr<ShaderProgram> shader);
	explicit EnergizerModel(EnergizerModel &model, glm::vec3 center, std::shared_ptr<ShaderProgram> shader);

	std::shared_ptr<AABB> GetAABB() override;

	// handlers
	void OnTick() override;
	void OnDraw() override;
	void OnCollide() override;

private:
	bool collected;

	std::unique_ptr<Renderer<SphereMesh>> renderer;
	glm::mat4 model_matrix;
};
