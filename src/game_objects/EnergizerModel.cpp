/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include <gtc/matrix_transform.inl>
#include "EnergizerModel.hpp"
#include "../ViewModel.h"

EnergizerModel::EnergizerModel(glm::vec3 center, std::shared_ptr<ShaderProgram> shader)
	: collected{false}
{
	model_matrix = glm::translate(glm::mat4(1.0f), center);
	model_matrix = scale(model_matrix, ENERGIZER_SCALE);
	renderer = std::make_unique<Renderer<SphereMesh>>(Renderer<SphereMesh>());
	renderer->Init(shader);
	EnergizerModel::GetAABB()->Move(center);
}

EnergizerModel::EnergizerModel(EnergizerModel& model, glm::vec3 center, std::shared_ptr<ShaderProgram> shader)
	: collected{false}
{
	model_matrix = glm::translate(glm::mat4(1.0f), center);
	model_matrix = scale(model_matrix, ENERGIZER_SCALE);
	renderer = std::make_unique<Renderer<SphereMesh>>(Renderer<SphereMesh>());
	renderer->getObject()->CopyVertices(*model.renderer->getObject());
	renderer->Init(shader);
	EnergizerModel::GetAABB()->Move(center);
}

std::shared_ptr<AABB> EnergizerModel::GetAABB()
{
	return renderer->getObject()->GetAABB();
}

void EnergizerModel::OnTick()
{
}

void EnergizerModel::OnDraw()
{
	if (!collected)
	{
		glm::mat4 temp = ViewModel::transform.model_matrix;
		ViewModel::transform.model_matrix = model_matrix;
		ViewModel::UpdateTransform();
		renderer->Draw();
		ViewModel::transform.model_matrix = temp;
		ViewModel::UpdateTransform();
	}
}

void EnergizerModel::OnCollide()
{
	collected = true;
	//std::cout << "Collected Energizer!" << std::endl;
	//TODO: play coin sound
}
