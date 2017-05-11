#pragma once
#include "PhysicsObject.h"
class RigidBody :
	public PhysicsObject
{
public:
	RigidBody();
	RigidBody(glm::vec3 position, glm::vec3 velocity, glm::vec3 rotation, float mass);
	RigidBody(glm::vec3 position, float angle, float speed, glm::vec3 rotation, float mass);

	virtual ~RigidBody();

	virtual void fixedUpdate(glm::vec3 gravity, float timeStep);
	virtual void debug();

	void applyForce(glm::vec3 a_force);
	void applYForceToActor(RigidBody * a_pActor2, glm::vec3 a_force);

	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getVelocity() { return m_velocity; }
	glm::vec3 getRotation() { return m_rotation; }
	float getMass() { return m_mass; }


protected:
	glm::vec3 m_velocity;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	float m_mass;


};

