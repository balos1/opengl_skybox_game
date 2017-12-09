/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <iostream>
#include "BoundingBox.h"

BoundingBox::BoundingBox()
	: initialized{false}, VAO{0}, vertex_position{0}, vertex_color{0}, vertex_normal{0}
{
}

void BoundingBox::DefineVertices(AABB* aabb)
{
	aabb->GetBox(this->vertices);
}

// TODO: This should take an AABB object instead, or at least have an overloaded option to.
// The reason is that this method encourages using this BoundingBox class as a wrapper for
// AABB, when it really should not be used that way. The class' purpose is just to 
// render the bounding box for debugging.
void BoundingBox::DefineVertices(std::vector<glm::vec3>& vertices)
{
	AABB aabb(vertices);
	aabb.GetBox(this->vertices);
}

bool BoundingBox::Init(std::shared_ptr<ShaderProgram>& shader)
{
	this->shader = shader;

	// check for required shader attributes
	try
	{
		vertex_position = shader->attribute("vertexPosition");
		vertex_color = shader->attribute("vertexColor");
	}
	catch (std::runtime_error)
	{
		std::cout << "ERROR: Cannot Init the BoundingBox object because required shader attributes were missing." << std::endl;
		return false;
	}

	// vertices must have at least one vertex
	if (vertices.size() == 0)
	{
		std::cout << "ERROR: Cannot Init the BoundingBox object because no vertices were defined." << std::endl;
		return false;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBOs.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs.vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertex_position);

	initialized = true;
	return true;
}

void BoundingBox::Draw(glm::vec4 color)
{
	if (initialized == false)
	{
		std::cout << "ERROR: Cannot render a BoundingBox object before it has been initialized." << std::endl;
		return;
	}

	glBindVertexArray(VAO);
	glVertexAttrib4f(vertex_color, color.r, color.g, color.b, color.a);
	glLineWidth(2);

	// first face
	unsigned int indices1[4] = { 0, 1, 3, 2 };
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, indices1);

	// second face
	unsigned int indices2[4] = { 1, 5, 7, 3 };
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, indices2);

	// third face
	unsigned int indices3[4] = { 5, 4, 6, 7 };
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, indices3);

	// fourth face
	unsigned int indices4[4] = { 4, 0, 2, 6 };
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, indices4);

	// fifth face
	unsigned int indices5[4] = { 4, 5, 1, 0 };
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, indices5);

	// sixth face
	unsigned int indices6[4] = { 2, 3, 7, 6 };
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, indices6);
}

