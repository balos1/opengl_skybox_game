/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "QuadMesh.hpp"

using namespace std;

QuadMesh::QuadMesh()
	: initialized{false}, VAO{0}, VBOs{0},
	  vertex_position{0}, vertex_color{0}, vertex_normal{0}
{
}


void QuadMesh::DefineVertexPositions()
{
	auto z = 0.0f;
	vertices.push_back(glm::vec3(-1.0f, 1.0f, z));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, z));
	vertices.push_back(glm::vec3(1.0f, 1.0f, z));
	vertices.push_back(glm::vec3(1.0f, -1.0f, z));
}

void QuadMesh::DefineVerticeColor(glm::vec4 color)
{
	this->color = color;
}

bool QuadMesh::Init(std::shared_ptr<ShaderProgram>& shader)
{
	this->shader = shader;

	// check for required shader attributes
	try
	{
		vertex_position = shader->attribute("vertexPosition");
		vertex_color = shader->attribute("vertexColor");
		vertex_normal = shader->attribute("vertexNormal");
	}
	catch (std::runtime_error)
	{
		// cannot Init because shader attributes are missing
		return false;
	}

	//Create one vertex array object
	glGenVertexArrays(1, &VAO);  
	glBindVertexArray(VAO);

	DefineVertexPositions();  

	// Create a buffer object for the vertices
	glGenBuffers(1, &VBOs.vertices); 
	glBindBuffer(GL_ARRAY_BUFFER, VBOs.vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// Do the shader plumbing here for this buffer
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr); 
	glEnableVertexAttribArray(vertex_position);

	DefineVerticeColor();	

	//Everything got initialized
	initialized = true;
	return true;  
}

void QuadMesh::Draw(glm::vec4 color)
{
	if (initialized == false)
		return;

	glBindVertexArray(VAO);
	glVertexAttrib4f(vertex_color, color.r, color.g, color.b, color.a);
	glVertexAttrib3f(vertex_normal, 0.0, 0.0, 1.0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}