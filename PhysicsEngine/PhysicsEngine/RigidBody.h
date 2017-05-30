#pragma once
#include "glm\glm.hpp"

struct RigidBodyData {
	glm::vec3 velocity;
	glm::vec3 angularVelocity;
	glm::vec3 startVelocity;
	glm::vec3 position;
	glm::vec3 startPosition;
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f); // how much rotation on each axis
	glm::mat4 rotationTransform = glm::mat4(1);
	float mass = 10.0f;
	bool onGround = false;
	bool isKinematic = false;
	bool rotationLock = true;
	// friction
	float linearDrag = 1.0f;	// 1.0f equals no drag
	float angularDrag = 0.45f;	// 1.0f equals no drag
	float elasticity = 0.5f;	// lower numbers are force absorbant
};


class RigidBody
{
public:
	RigidBody();
	RigidBody(glm::vec3 position, glm::vec3 velocity, glm::vec3 rotation, float mass);
	RigidBody(glm::vec3 position, float angle, float speed, glm::vec3 rotation, float mass);

	~RigidBody();

	void fixedUpdate(glm::vec3 gravity, float timeStep);
	void debug();

	RigidBodyData data;
	// force application
	void applyForce(glm::vec3 a_force);
	void applyForceToActor(RigidBody * a_pActor2, glm::vec3 a_force);
	void applyTorque(glm::vec3 a_force);
	void applyTorqueToActor(RigidBody * a_pActor2, glm::vec3 a_force);
	// position predciction
	glm::vec3 predictPosition(float a_time, float a_angle, float a_speed, glm::vec3 a_gravity);
	glm::vec3 predictPosition(float a_time, glm::vec3 a_velocity, glm::vec3 a_gravity);
	glm::vec3 predictPosition(float a_time, glm::vec3 a_gravity);
protected:
	glm::vec3 m_startPosition;
};

