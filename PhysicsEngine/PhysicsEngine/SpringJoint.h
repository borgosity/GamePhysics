#pragma once
#include "PhysicsObject.h"

class RigidBody;

class SpringJoint :
	public PhysicsObject
{
public:
	SpringJoint(RigidBody * connectionA, RigidBody * connectionB, float spring, float damping, bool twoD = false);
	~SpringJoint();
private:
	void virtual fixedUpdate(glm::vec3 gravity, float timeStep);
	void virtual debug();
	void virtual makeGizmo();

	bool m_2D;
	RigidBody * m_connections[2];
	float m_damping;
	float m_restLength;
	float m_spring;
};

