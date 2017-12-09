/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.inl>
#include <detail/type_mat.hpp>
#include <gtc/type_ptr.inl>
#include "StageModel.hpp"
#include "../Transformation.h"
#include "../ViewModel.h"
#include <algorithm>


StageModel::StageModel()
{
	// 8 hallways for now.
	for (auto i = 0; i < 8; ++i)
		hallways.push_back(HallwayModel());

	// requires 4 elbows
	for (auto i = 0; i < 4; ++i)
		elbows.push_back(HallwayModel());

	// 12 t3s
	for (auto i = 0; i < 12; ++i)
		t3s.push_back(HallwayModel());

	// 4 t4s
	for (auto i = 0; i < 4; ++i)
		t4s.push_back(HallwayModel());
}

bool StageModel::Init(std::shared_ptr<ShaderProgram>& shader)
{
	this->shader = shader;

	 //Init the first hallway normally, but then copy
	 //the vertices from it for the other 7
	hallways.front().Init(shader, STRAIGHT);
	for (auto i = 1; i < hallways.size(); ++i)
	{
		hallways.at(i).CopyVertices(hallways.front());
		if (hallways.at(i).Init(shader, STRAIGHT) == false)
		{
			std::cout << "Count not Init hallways." << std::endl;
			return false;
		}
	}

	// all the elbows we need
	elbows.front().Init(shader, ELBOW);
	for (auto i = 0; i < elbows.size(); ++i)
	{
		elbows.at(i).CopyVertices(elbows.front());
		if (elbows.at(i).Init(shader, ELBOW) == false)
		{
			std::cout << "Count not Init hallway elbows." << std::endl;
			return false;
		}
	}

	// all the T3s we need
	t3s.front().Init(shader, T3);
	for (auto i = 0; i < t3s.size(); ++i)
	{
		t3s.at(i).CopyVertices(t3s.front());
		if (t3s.at(i).Init(shader, T3) == false)
		{
			std::cout << "Count not Init hallway t3s." << std::endl;
			return false;
		}
	}

	// all the T4s we need
	t4s.front().Init(shader, T4);
	for (auto i = 0; i < t4s.size(); ++i)
	{
		t4s.at(i).CopyVertices(t4s.front());
		if (t4s.at(i).Init(shader, T4) == false)
		{
			std::cout << "Count not Init hallways t4s." << std::endl;
			return false;
		}
	}

	// TODO: Rescale .obj pieces to be on the same scale
	// Here I must perform calculations to find the scaling factor for each type of hallway piece
	// this is necessary since the .obj models are not all scaled the same.
	GLfloat hall_width  = hallways.front().Width();
	GLfloat elbow_width = elbows.front().Width();
	GLfloat t3_width    = t3s.front().Width();
	GLfloat t4_width    = t4s.front().Width();
	GLfloat hall_height = hallways.front().Height();
	GLfloat elbow_height= elbows.front().Height();
	GLfloat t3_height   = t3s.front().Height();
	GLfloat t4_height   = t4s.front().Height();
	GLfloat hall_length = hallways.front().Length();
	GLfloat elbow_length= elbows.front().Length();
	GLfloat t3_length   = t3s.front().Length();
	GLfloat t4_length   = t4s.front().Length();

	hallway_scale.x = HALL_LENGTH / hall_length;
	hallway_scale.y = HALL_HEIGHT / hall_height;
	hallway_scale.z = HALL_WIDTH / hall_width;

	elbow_scale.x = HALL_LENGTH / elbow_length;
	elbow_scale.y = HALL_HEIGHT / elbow_height;
	elbow_scale.z = HALL_WIDTH / elbow_width;

	t3_scale.x = HALL_LENGTH / t3_length;
	t3_scale.y = HALL_HEIGHT / t3_height;
	t3_scale.z = HALL_WIDTH / t3_width;

	t4_scale.x = HALL_LENGTH / t4_length;
	t4_scale.y = HALL_HEIGHT / t4_height;
	t4_scale.z = HALL_WIDTH / t4_width;

	//GLfloat max_width = std::max(hall_width, elbow_width);
	//max_width = std::max(max_width, t3_width);
	//max_width = std::max(max_width, t4_width);

	//GLfloat max_height = std::max(hall_height, elbow_height);
	//max_height = std::max(max_height, t3_height);
	//max_height = std::max(max_height, t4_height);

	//GLfloat max_length = std::max(hall_length, elbow_length);
	//max_length = std::max(max_length, t3_length);
	//max_length = std::max(max_length, t4_length);

	//if (hall_width == max_width)
	//	hallway_scale.z = HALL_WIDTH / hall_width;
	//else if (hall_length == max_length)
	//	hallway_scale.x = HALL_LENGTH / hall_length;
	//else if (hall_height == max_height)
	//	hallway_scale.y = HALL_HEIGHT / hall_height;

	//if (elbow_width == max_width)
	//	elbow_scale.z = HALL_WIDTH / elbow_width;
	//else if (elbow_length == max_length)
	//	elbow_scale.x = HALL_LENGTH / elbow_length;
	//else if (elbow_height == max_height)
	//	elbow_scale.y = HALL_HEIGHT / hall_height;

	//if (t3_width == max_width)
	//	t3_scale.z = HALL_WIDTH / t3_width;
	//else if (t3_length == max_length)
	//	t3_scale.x = HALL_LENGTH / t3_length;
	//else if (t3_height == max_height)
	//	t3_scale.y = HALL_HEIGHT / t3_height;

	//if (t4_width == max_width)
	//	t4_scale.z = HALL_WIDTH / t4_width;
	//else if (t4_length == max_length)
	//	t4_scale.x = HALL_LENGTH / t4_length;
	//else if (t4_height == max_height)
	//	t4_scale.y = HALL_HEIGHT / t4_height;

	return true;
}

void StageModel::Draw(glm::vec4 color)
{
	const GLfloat scaleFactor = 2.0f;
	const GLfloat yTranslate = 0.25f;

	
	// this corridor lies ont the x-axis
	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = glm::rotate(ViewModel::transform.model_matrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, 0.0f));
	//UpdateTransform();
	//hallways[0].Draw();	 // red

	//ViewModel::transform.model_matrix = scale(mat4(1.0f), glm::vec3(scaleFactor));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
	//ViewModel::UpdateTransform();
	//hallways[0].Draw();

	//ViewModel::transform.model_matrix = scale(mat4(1.0f), elbow_scale);
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(-10.0f, yTranslate, 0.0f));
	//ViewModel::UpdateTransform();
	//elbows[0].Draw();

	//ViewModel::transform.model_matrix = scale(mat4(1.0f), glm::vec3(scaleFactor));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, 0.0f));
	//ViewModel::UpdateTransform();
	//t3s[0].Draw();

	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = glm::rotate(ViewModel::transform.model_matrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, -hallways[1].Length() + -hallways[1].Width()));
	//UpdateTransform();
	//hallways[1].Draw();  // blue

	//// this corridor lies on the z-axis 
	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, 0.0f));
	//UpdateTransform();
	//hallways[2].Draw();  // green

	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, -hallways[3].Length()));
	//UpdateTransform();
	//hallways[3].Draw();  // black

	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(diagnoalScaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = glm::rotate(ViewModel::transform.model_matrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, 0.0f));
	//UpdateTransform();
	//hallways[4].Draw(); // purple - diagonal

	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = glm::rotate(ViewModel::transform.model_matrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, hallways[5].Length() - hallways[5].Width()));
	//UpdateTransform();
	//hallways[5].Draw(); // yellow - 180 deg of blue

	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(0.0f, yTranslate, -hallways[6].Length()));
	//UpdateTransform();
	//hallways[6].Draw();  // grey - opposite translate of black
	//

	//ViewModel::transform.model_matrix = scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	//ViewModel::transform.model_matrix = glm::rotate(ViewModel::transform.model_matrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ViewModel::transform.model_matrix = translate(ViewModel::transform.model_matrix, glm::vec3(hallways[7].Length(), yTranslate, 0.0f));
	//UpdateTransform();
	//hallways[7].Draw();  // orange - positive translate of red
}