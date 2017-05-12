#pragma once
#include <vector>
#include "glm\glm.hpp"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void update(float a_dt);
	void updateGizmos();
	void debugScene();
	// actor functions
	void addActor(PhysicsObject * a_actor);
	void removeActor(PhysicsObject * a_actor);
	void resetScene();
	void clearScene();

	void setGravity(const glm::vec3 a_gravity) { m_gravity = a_gravity; }
	glm::vec3 setGravity() const { return m_gravity; }
	void setTimeStep(const float a_timeStep) { m_timeStep = a_timeStep; }
	float setTimeStep() const { return m_timeStep; }

protected:
	glm::vec3	m_gravity;
	float		m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	// gui options
	bool m_applyForce;
	int m_iActorA;
	PhysicsObject* m_pActorA = nullptr;
	int m_iActorB;
	PhysicsObject* m_pActorB = nullptr;
};

