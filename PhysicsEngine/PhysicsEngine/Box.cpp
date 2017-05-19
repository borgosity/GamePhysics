#include "Box.h"
#include "Gizmos.h"
#include "RigidBody.h"

Box::Box(glm::vec3 position, glm::vec3 velocity, float mass, float size, glm::vec4 colour, bool twoD)
{
	m_shapeID = BOX;
	m_size = size;
	m_colour = colour;
	m_rigidbody = new RigidBody(position, velocity, glm::vec3(0.0f, 0.0f, 1.0f), mass);
	m_2D = twoD;
}

Box::Box(glm::vec3 position, float angle, float speed, float mass, float size, glm::vec4 colour, bool twoD)
{
	m_shapeID = BOX;
	m_size = size;
	m_colour = colour;
	m_rigidbody = new RigidBody(position, angle, speed, glm::vec3(0.0f, 0.0f, 1.0f), mass);
	m_2D = twoD;
}

Box::~Box()
{
	delete m_rigidbody;
}

void Box::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	m_rigidbody->fixedUpdate(gravity, timeStep);
}

void Box::debug()
{
	m_rigidbody->debug();
}

void Box::makeGizmo()
{
	if (m_2D) {
		aie::Gizmos::add2DAABBFilled(glm::vec2(m_rigidbody->data.position.x, m_rigidbody->data.position.y), glm::vec2(m_size), m_colour);
	}
	else {
		aie::Gizmos::addAABBFilled(m_rigidbody->data.position, glm::vec3(m_size), m_colour);
	}
}
