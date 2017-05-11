#include "Sphere.h"
#include "Gizmos.h"


Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour) : RigidBody(position, velocity, glm::vec3(0.0f, 0.0f, 1.0f), mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::Sphere(glm::vec3 position, float angle, float speed, float mass, float radius, glm::vec4 colour) : RigidBody(position, angle, speed, glm::vec3(0.0f, 0.0f, 1.0f), mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	//aie::Gizmos::add2DCircle(glm::vec2(m_position.x, m_position.y), m_radius, 12, m_colour);
	aie::Gizmos::addSphere(m_position, m_radius, 12, 12, m_colour);
}
