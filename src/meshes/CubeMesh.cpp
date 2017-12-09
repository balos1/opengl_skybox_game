
#include <glew.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/type_ptr.inl>
#include "CubeMesh.hpp"
#include "../config.inl"
#include "../utils/ObjLoader.hpp"


CubeMesh::CubeMesh()
	: initialized{false}, VBOs{0}, VAO{0}, 
	  vertex_position{0}, vertex_color{0}, vertex_normal{0},
	  render_aabb{__DEBUG__}
{
}

void CubeMesh::DefineVertices()
{
	if (vertices.size() != 0)
		return; 

	ObjLoader obj_loader;
	obj_loader
		.LoadObj(RES_PATH + "/cube.obj")
		.Map(&vertices, &texcoords, &normals);
}

void CubeMesh::DefineColors()
{
}

void CubeMesh::CopyVertices(CubeMesh& model)
{
	vertices.insert(vertices.begin(), model.vertices.begin(), model.vertices.end());
	normals.insert(normals.begin(), model.normals.begin(), model.normals.end());
}

bool CubeMesh::Init(std::shared_ptr<ShaderProgram>& shader)
{
	this->shader = shader;

	// check for required shader attributes/uniforms
	try
	{
		vertex_position   = shader->attribute("vertexPosition");
		vertex_color      = shader->attribute("vertexColor");
		vertex_normal     = shader->attribute("vertexNormal");
	}
	catch (std::runtime_error)
	{
		std::cout << "ERROR: Cannot Init the CubeMesh object because required shader attributes were missing." << std::endl;
		return false;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	DefineVertices();

	glGenBuffers(2, &VBOs.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs.vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertex_position);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs.normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_normal, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertex_normal);

	DefineColors();

	// Init a bounding box
	aabb = std::make_shared<AABB>(AABB(vertices));

	if (render_aabb)
	{
		boundingBox.getObject()->DefineVertices(aabb.get());
		boundingBox.Init(shader);
	}

	initialized = true;
	return true;
}

void CubeMesh::Draw(glm::vec4 color)
{
	if (initialized == false)
	{
		std::cout << "ERROR: Cannot render a CubeMesh object before it has been initialized." << std::endl;
		return;
	}

	glBindVertexArray(VAO);
	glVertexAttrib4f(vertex_color, color.r, color.g, color.b, color.a);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// also draw bounding box if applicable
	if (render_aabb)
		boundingBox.Draw();
}

void CubeMesh::ShowBoundingBox(bool value)
{
	// if the previous setting for render_aabb was false,
	// reinit the BoundingBox object
	if (render_aabb == false && value == true)
	{
		boundingBox.getObject()->DefineVertices(aabb.get());
		boundingBox.Init(shader);
	}
		
	render_aabb = value;
}

std::shared_ptr<AABB> CubeMesh::GetAABB() const
{
	return aabb;
}
