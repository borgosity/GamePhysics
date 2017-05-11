#pragma once
#include "RigidBody.h"
class Sphere :
	public RigidBody
{
public:
	Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour);
	Sphere(glm::vec3 position, float angle, float speed, float mass, float radius, glm::vec4 colour);
	virtual ~Sphere();

	virtual void makeGizmo() ;
	
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }
private:
	float m_radius;
	glm::vec4 m_colour;
};

