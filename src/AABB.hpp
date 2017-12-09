#pragma once

#include <vector>
#include <glm.hpp>
#include "Transformation.h"
#include <detail/type_mat.hpp>


class AABB
{
public:
	AABB();
	explicit AABB(std::vector<glm::vec3>& vertices);
	
	void Init(std::vector<glm::vec3> &vertices);
	bool AABBtoAABB(const AABB &otherbox) const;
	static bool AABBtoAABB(const AABB &box1, const AABB &box2);
	void Move(glm::vec3 displacement);
	glm::vec3 Size() const { return max - min; }
	void GetBox(std::vector<glm::vec3>& vertices) const;

private:
	glm::vec3 max;
	glm::vec3 min;

};
