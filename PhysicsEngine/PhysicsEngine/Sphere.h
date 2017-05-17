#pragma once
#include "PhysicsObject.h"
class Sphere :
	public PhysicsObject
{
public:
	Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour, bool twoD = false);
	Sphere(glm::vec3 position, float angle, float speed, float mass, float radius, glm::vec4 colour, bool twoD = false);
	virtual ~Sphere();

	virtual void fixedUpdate(glm::vec3 gravity, float tiemStep);
	virtual void debug();
	virtual void makeGizmo();
	
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }
private:
	float m_radius;
	glm::vec4 m_colour;
};

