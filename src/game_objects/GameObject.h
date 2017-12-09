#pragma once

#include "../AABB.hpp"
#include <memory>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() {}
	
	virtual std::shared_ptr<AABB> GetAABB() = 0;

	glm::vec3 displacement;

	// event handlers for all the events that can occur in game
	virtual void OnStart();
	virtual void OnTick();
	virtual void OnDraw();
	virtual void OnCollide();
	virtual void OnDeath();
	virtual void OnCaptured();
	virtual void OnEnd();
};
