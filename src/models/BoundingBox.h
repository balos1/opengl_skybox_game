#pragma once

#include "../AABB.hpp"
#include "../Renderable.hpp"
#include "../VboSet.h"

class BoundingBox : public Renderable
{
public:
	BoundingBox();
	void DefineVertices(AABB *aabb);
	void DefineVertices(std::vector<glm::vec3>& vertices);
	bool BoundingBox::Init(std::shared_ptr<ShaderProgram>& shader) override;
	void Draw(glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)) override;

private:
	// state
	bool initialized;

	// OpenGL fields
	VboSet VBOs;
	GLuint VAO;

	// required shader attributes
	GLuint vertex_position;
	GLuint vertex_color;
	GLuint vertex_normal;

	// properties
	std::vector<glm::vec3> vertices;
	
};
