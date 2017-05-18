#include "Sphere.h"
#include "Gizmos.h"
#include "RigidBody.h"


Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour, bool twoD)
{
	m_shapeID = SPHERE;
	m_radius = radius;
	m_colour = colour;
	m_rigidbody = new RigidBody(position, velocity, glm::vec3(0.0f, 0.0f, 1.0f), mass);
	m_2D = twoD;
}

Sphere::Sphere(glm::vec3 position, float angle, float speed, float mass, float radius, glm::vec4 colour, bool twoD)
{
	m_shapeID = SPHERE;
	m_radius = radius;
	m_colour = colour;
	m_rigidbody = new RigidBody(position, angle, speed, glm::vec3(0.0f, 0.0f, 1.0f), mass);
	m_2D = twoD;
}

Sphere::~Sphere()
{
	delete m_rigidbody;
}

void Sphere::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	m_rigidbody->fixedUpdate(gravity, timeStep);
}

void Sphere::debug()
{
	m_rigidbody->debug();
}

void Sphere::makeGizmo()
{
	if (m_2D) {
		aie::Gizmos::add2DCircle(glm::vec2(m_rigidbody->data.position.x, m_rigidbody->data.position.y), m_radius, 12, m_colour);
	}
	else {
		aie::Gizmos::addSphere(m_rigidbody->data.position, m_radius, 12, 12, m_colour);
	}
}
