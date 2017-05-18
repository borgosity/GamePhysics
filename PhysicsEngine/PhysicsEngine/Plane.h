#pragma once
#include "PhysicsObject.h"




class Plane :
	public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec3 normal, float distance, bool twoD = false);
	virtual ~Plane();

	virtual void fixedUpdate(glm::vec3 gravity, float timeStep);
	virtual void debug();
	virtual void makeGizmo();
	// get/set
	virtual glm::vec3 getPosition() override { return m_position; };
	//virtual  void setPosition(glm::vec3 a_position) override;
	glm::vec3 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

private:
	bool m_2D;
	glm::vec3 m_position;
	glm::vec3 m_normal;
	float m_distanceToOrigin;
};

