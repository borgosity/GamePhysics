#pragma once
#include "PhysicsObject.h"
class Plane :
	public PhysicsObject
{
public:
	Plane();
	virtual ~Plane();

protected:
	glm::vec3 m_normal;
	glm::vec3 m_distanceToOrigin;
};

