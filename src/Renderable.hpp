#pragma once

#include <memory>
#include "utils/ShaderProgram.hpp"


class Renderable
{
public:
	virtual ~Renderable() {}
	virtual bool Init(std::shared_ptr<ShaderProgram>& shader) = 0;
	virtual void Draw(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) = 0;
protected:
	std::shared_ptr<ShaderProgram> shader;
};
