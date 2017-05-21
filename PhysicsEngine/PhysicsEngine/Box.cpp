#include "Box.h"
#include "Gizmos.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"

#include <stdlib.h>
#include "glm\glm.hpp"
#include <glm/gtc/type_ptr.hpp>

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
/// testing AABB intersection with another AABB
/// - uses the center point/radius method
bool Box::testAABB(PhysicsObject * a_object)
{	
	if (a_object->getShapeID() == BOX) {
		Box * box = (Box*)a_object;
		// cache data
		glm::vec3 thisPos = getPosition();
		glm::vec3 otherPos = box->getPosition();
		float otherSize = box->getSize();
		// perform checks
		if (abs(thisPos.x - otherPos.x) > (m_size + otherSize)) return false;
		if (abs(thisPos.y - otherPos.y) > (m_size + otherSize)) return false;
		if (abs(thisPos.z - otherPos.z) > (m_size + otherSize)) return false;
		// return true is all checks pass
		return true;
	}
	if (a_object->getShapeID() == SPHERE) {
		Sphere * sphere = (Sphere*)a_object;
		float radius = sphere->getRadius();
		// get the squared distance bewtween the center point and the AABB
		float distance = distToPointAABB(sphere->getPosition());
		// perform checks
		if (distance > radius * radius) return false;
		// return true is all checks pass
		return true;
	}
	if (a_object->getShapeID() == PLANE) {
		Plane * plane = (Plane *)a_object;
		// cache some values
		glm::vec3 planeNormal = plane->getNormal();
		glm::vec3 center = getPosition();
		glm::vec3 extents(getSize());
		// Compute the projection interval radius of box onto the plane
		float radius = extents.x * abs(planeNormal.x) + extents.y * abs(planeNormal.y) + extents.z * abs(planeNormal.z);
		// Compute distance of box center from plane
		float distance = dot(planeNormal, center);
		// Intersection occurs when distance s falls within [-r,+r] interval
		if (abs(distance) > radius) return false;
		// return true is all checks pass
		return true;
	}
	return false;
}

float Box::distToPointAABB(glm::vec3 a_point)
{
	glm::vec3 center = getPosition();
	float distance = 0.0f;
	glm::vec3 min(center.x - m_size, center.y - m_size, center.z - m_size);
	glm::vec3 max(center.x + m_size, center.y + m_size, center.z + m_size);
	
	for (int i = 0; i < 3; i++)
	{
		float v = a_point[i];
		if (v < min[i]) distance += (min[i] - v) * (min[i] - v);
		if (v > max[i]) distance += (v - max[i]) * (v - max[i]);
	}
	return distance;
}
