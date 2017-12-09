/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#include "utils/ShaderProgram.hpp"

template <class T>
class Renderer
{
protected:
	std::shared_ptr<ShaderProgram> program;
	T theObject;

public:
	Renderer() {}

	T *getObject() { return &theObject; }

	bool Init(std::shared_ptr<ShaderProgram>& program)
	{
		this->program = program;
		this->program->use();
		return theObject.Init(program);
	}

	void Draw(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
	{
		program->use();
		theObject.Draw(color);
	}
};