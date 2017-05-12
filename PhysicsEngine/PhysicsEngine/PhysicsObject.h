#pragma once
#include "glm\glm.hpp"


enum ShapeType {
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
};

class RigidBody;

class PhysicsObject
{
protected:
	PhysicsObject();
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
public:
	virtual void fixedUpdate(glm::vec3 gravity, float tiemStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;

	virtual glm::vec3 getPosition();
	virtual void resetPosition();
	virtual void resetVelocity();
	RigidBody * rigidbody() { return m_rigidbody; }

protected:
	ShapeType m_shapeID;
	RigidBody * m_rigidbody = nullptr;
};

