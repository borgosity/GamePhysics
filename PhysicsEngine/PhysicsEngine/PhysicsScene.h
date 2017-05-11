#pragma once
#include <vector>
#include "glm\glm.hpp"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject * a_actor);
	void removeActor(PhysicsObject * a_actor);
	void update(float a_dt);
	void updateGizmos();
	void debugScene();

	void setGravity(const glm::vec3 a_gravity) { m_gravity = a_gravity; }
	glm::vec3 setGravity() const { return m_gravity; }
	void setTimeStep(const float a_timeStep) { m_timeStep = a_timeStep; }
	float setTimeStep() const { return m_timeStep; }

protected:
	glm::vec3	m_gravity;
	float		m_timeStep;
	std::vector<PhysicsObject*> m_actors;

};

