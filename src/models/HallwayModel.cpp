#include "HallwayModel.hpp"
#include "../utils/ObjLoader.hpp"
#include "../config.inl"
#include <iostream>


HallwayModel::HallwayModel()
	: initialized{false}, VAO{0}, 
	  vertex_position{0}, vertex_color{0}, vertex_normal{0}, 
      render_aabb{__DEBUG__}
{
}

void HallwayModel::DefineVertices(HallwayTypes type)
{
	if (vertices.size() != 0)
		return;

	std::string objName;

	if (type == STRAIGHT)
		objName = "/hallway-long.obj";
	else if (type == ELBOW)
		objName = "/hallway-elbow.obj";
	else if (type == T3)
		objName = "/hallway-t3.obj";
	else if (type == T4)
		objName = "/hallway-t4.obj";
	else
		return;

	ObjLoader obj_loader;
	obj_loader
		.LoadObj(RES_PATH + objName)
		.Map(&vertices, &texcoords, &normals);
}

GLfloat HallwayModel::Length() const
{
	return Size().x;
}

GLfloat HallwayModel::Width() const
{
	return Size().z;
}

GLfloat HallwayModel::Height() const
{
	return Size().y;
}

void HallwayModel::CopyVertices(HallwayModel& model)
{
	vertices.insert(vertices.begin(), model.vertices.begin(), model.vertices.end());
	normals.insert(normals.begin(), model.normals.begin(), model.normals.end());
}

bool HallwayModel::Init(std::shared_ptr<ShaderProgram>& shader, HallwayTypes type)
{
	this->shader = shader;

	// check for required shader attributes/uniforms
	try
	{
		vertex_position = shader->attribute("vertexPosition");
		vertex_color = shader->attribute("vertexColor");
		vertex_normal = shader->attribute("vertexNormal");
	}
	catch (std::runtime_error)
	{
		std::cout << "ERROR: Cannot Init the HallwayModel object because required shader attributes were missing." << std::endl;
		return false;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	DefineVertices(type);

	glGenBuffers(2, &VBOs.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs.vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertex_position);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs.normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_normal, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertex_normal);

	// Init a bounding box
	aabb = std::make_unique<AABB>(AABB(vertices));

	if (render_aabb)
	{
		boundingBox.getObject()->DefineVertices(aabb.get());
		boundingBox.Init(shader);
	}

	initialized = true;
	return true;
}

void HallwayModel::Draw(glm::vec4 color)
{
	if (initialized == false)
	{
		std::cout << "ERROR: Cannot render a HallwayModel object before it has been initialized." << std::endl;
		return;
	}

	glBindVertexArray(VAO);
	glVertexAttrib4f(vertex_color, color.r, color.g, color.b, color.a);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// also draw bounding box if applicable
	if (render_aabb)
		boundingBox.Draw();
}

void HallwayModel::ShowBoundingBox(bool value)
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
