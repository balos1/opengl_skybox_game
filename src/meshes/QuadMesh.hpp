/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <glm.hpp>
#include <vector>
#include <memory>
#include "../utils/ShaderProgram.hpp"
#include "../Renderable.hpp"
#include "../VboSet.h"


class QuadMesh : public Renderable
{
public:
	QuadMesh();
	virtual ~QuadMesh() {}

	virtual void DefineVertexPositions();
	virtual void DefineVerticeColor(glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0));
	bool Init(std::shared_ptr<ShaderProgram>& shader) override;
	void Draw(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) override;

protected:
	// state
	bool initialized;

	// OpenGL fields/structures
	GLuint VAO;
	VboSet VBOs;

	// required shader attributes
	GLuint vertex_position;
	GLuint vertex_color;
	GLuint vertex_normal;
	
	// properties
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::vec4 color;
};
