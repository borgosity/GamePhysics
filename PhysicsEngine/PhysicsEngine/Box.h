#pragma once
#include "PhysicsObject.h"
class Box :
	public PhysicsObject
{
public:
	Box(glm::vec3 position, glm::vec3 velocity, float mass, float size, glm::vec4 colour, bool twoD = false);
	Box(glm::vec3 position, float angle, float speed, float mass, float size, glm::vec4 colour, bool twoD = false);
	virtual ~Box();

	virtual void fixedUpdate(glm::vec3 gravity, float timeStep);
	virtual void debug();
	virtual void makeGizmo();

	float getSize() { return m_size; }
	glm::vec4 getColour() { return m_colour; }
private:
	float m_size;
	glm::vec4 m_colour;
};

