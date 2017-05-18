#pragma once
#include "glm\glm.hpp"


enum ShapeType {
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	SHAPE_COUNT = 2
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
	// get /set
	int getShapeID() { return m_shapeID; }
	virtual glm::vec3 getPosition();
	virtual glm::vec3 getVelocity();

	virtual void setPosition(glm::vec3 a_position);
	virtual void setVelocity(glm::vec3 a_velocity);
	RigidBody * rigidbody() { return m_rigidbody; }
	bool toggle2D() { m_2D = !m_2D; return m_2D; }

	virtual void resetPosition();
	virtual void resetVelocity();

protected:
	ShapeType m_shapeID;
	RigidBody * m_rigidbody = nullptr;
	bool m_2D;
};

