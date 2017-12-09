#include <algorithm>
#include "AABB.hpp"
#include "ViewModel.h"

AABB::AABB()
{
}

AABB::AABB(std::vector<glm::vec3> &vertices)
{
	Init(vertices);
}

void AABB::Init(std::vector<glm::vec3>  &vertices)
{
	min = max = glm::vec3(0.0f, 0.0f, 0.0f);
	for (const auto& vertex : vertices)
	{
		max.x = std::max(vertex.x, max.x);
		min.x = std::min(vertex.x, min.x);
		max.y = std::max(vertex.y, max.y);
		min.y = std::min(vertex.y, min.y);
		max.z = std::max(vertex.z, max.z);
		min.z = std::min(vertex.z, min.z);
	}
}

bool AABB::AABBtoAABB(const AABB& otherbox) const
{
	return AABBtoAABB(*this, otherbox);
}

bool AABB::AABBtoAABB(const AABB& box1, const AABB& box2)
{
	// return true if box1 intersects box 2 at all
	// this case is signaled by box1's max being greater than box2's min
	// and box1's min is less than box2's min in every direction (x,y,z)
	/*return (box1.max.x > box2.min.x &&
			box1.min.x < box2.max.x &&
			box1.max.y > box2.min.y &&
			box1.min.y < box2.min.y &&
			box1.max.z > box2.min.z &&
			box1.min.z < box2.max.z);*/

	bool test = box1.max.x > box2.min.x;
	test = test && box1.min.x < box2.max.x;
	test = test && box1.max.y > box2.min.y;
	test = test && box1.min.y < box2.max.y;
	test = test && box1.max.z > box2.min.z;
	test = test && box1.min.z < box2.max.z;
	return test;
}

/**
* \brief Moves the AABB one movement unit in the specified direction.
*/
void AABB::Move(glm::vec3 displacement)
{
	max  = max + displacement;
    min  = min + displacement;
}

/**
 * \brief Fill a vertices array with the bounding box vertices for the AABB.
 *		 
 * There are 8 vertices that are in bottom left, bottom right, top left,
 * top right, front first, back second order. 
 * http://stackoverflow.com/questions/8142388/in-what-order-should-i-send-my-vertices-to-opengl-for-culling
 */
void AABB::GetBox(std::vector<glm::vec3>& vertices) const
{
	vertices.push_back(glm::vec3(min.x, min.y, max.z));	//0
	vertices.push_back(glm::vec3(max.x, min.y, max.z));	//1
	vertices.push_back(glm::vec3(min.x, max.y, max.z));	//2
	vertices.push_back(glm::vec3(max.x, max.y, max.z));	//3
	vertices.push_back(glm::vec3(min.x, min.y, min.z));	//4
	vertices.push_back(glm::vec3(max.x, min.y, min.z));	//5
	vertices.push_back(glm::vec3(min.x, max.y, min.z));	//6
	vertices.push_back(glm::vec3(max.x, max.y, min.z));	//7
}