#pragma once

#include <vector>
#include <glm.hpp>
#include "../Renderable.hpp"
#include "../VboSet.h"
#include "../Renderer.h"
#include "../models/BoundingBox.h"

class CubeMesh : public Renderable
{
public:
	CubeMesh();
	virtual void DefineVertices();
	virtual void DefineColors();
	void CopyVertices(CubeMesh &model);
	bool Init(std::shared_ptr<ShaderProgram>& shader) override;
	void Draw(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) override;
	void ShowBoundingBox(bool value);
	std::shared_ptr<AABB> GetAABB() const;
	glm::vec3 Size() const { return aabb->Size(); }

protected:
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
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec4> colors;
	glm::vec4 static_color;
	std::shared_ptr<AABB> aabb;
	Renderer<BoundingBox> boundingBox;
	bool render_aabb;
};
