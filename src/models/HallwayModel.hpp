#pragma once

#include "../meshes/CubeMesh.hpp"

enum HallwayTypes
{
	STRAIGHT, ELBOW, T3, T4
};


class HallwayModel
{
public:
	HallwayModel();
	void DefineVertices(HallwayTypes type);
	GLfloat Length() const;
	GLfloat Width() const;
	GLfloat Height() const;
	void CopyVertices(HallwayModel &model);
	bool Init(std::shared_ptr<ShaderProgram>& shader, HallwayTypes);
	void Draw(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	void ShowBoundingBox(bool value);
	glm::vec3 Size() const { return aabb->Size(); }

protected:
	// state
	bool initialized;

	// OpenGL fields
	VboSet VBOs;
	GLuint VAO;

	// required shader attributes
	std::shared_ptr<ShaderProgram> shader;
	GLuint vertex_position;
	GLuint vertex_color;
	GLuint vertex_normal;

	// properties
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec4> colors;
	glm::vec4 static_color;
	std::unique_ptr<AABB> aabb;
	Renderer<BoundingBox> boundingBox;
	bool render_aabb;
};

