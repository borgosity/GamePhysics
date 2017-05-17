#include "PhysicsObject.h"
#include "RigidBody.h"



PhysicsObject::PhysicsObject()
{
}

glm::vec3 PhysicsObject::getPosition()
{
	if (m_rigidbody != nullptr) {
		return m_rigidbody->data.position;
	}
	else {
		return glm::vec3(-1.0f);
	}
}

void PhysicsObject::setVelocity(glm::vec3 a_velocity)
{
	if (m_rigidbody != nullptr) {
		m_rigidbody->data.velocity = a_velocity;
	}
}

void PhysicsObject::resetPosition()
{
	if (m_rigidbody != nullptr) {
		m_rigidbody->data.position = m_rigidbody->data.startPosition;
	}
}

void PhysicsObject::resetVelocity()
{
	if (m_rigidbody != nullptr) {
		m_rigidbody->data.velocity = m_rigidbody->data.startVelocity;
	}
}

