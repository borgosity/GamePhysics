#include "RigidBody.h"
#include <string>
// gui
#include "imgui_glfw3.h"
#include <glm/gtc/type_ptr.hpp>


RigidBody::RigidBody()
{
}

RigidBody::RigidBody(glm::vec3 position, glm::vec3 velocity, glm::vec3 rotation, float mass)
{
	data.position = position;
	data.startPosition = position;
	data.velocity = velocity;
	data.startVelocity = velocity;
	data.rotation = rotation;
	data.mass = mass;
}

RigidBody::RigidBody(glm::vec3 position, float angle, float speed, glm::vec3 rotation, float mass)
{
	data.position = position;
	data.startPosition = position;
	data.velocity = glm::vec3(speed * cos(angle), speed * sin(angle), 0.0f);
	data.startVelocity = data.velocity;
	data.rotation = rotation;
	data.mass = mass;
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	glm::vec3 totalGavity(data.onGround ? glm::vec3(0.0f) : gravity);
	applyForce(totalGavity * data.mass * timeStep);
	if (data.isKinematic && data.onGround) {
		data.velocity *= data.linearDrag;
	}
	data.position += data.velocity * timeStep;
}

void RigidBody::debug()
{	// create agent window
	std::string windowName = "RigidBody";
	ImGui::Text(windowName.c_str());
	ImGui::DragFloat3("position", glm::value_ptr(data.position), 0.05f, -100.0f, 100.0f, "%.2f");
	ImGui::DragFloat3("velocity", glm::value_ptr(data.velocity), 0.05f, -100.0f, 100.0f, "%.2f");
	ImGui::DragFloat3("rotation", glm::value_ptr(data.rotation), 0.05f, -100.0f, 100.0f, "%.2f");
	ImGui::DragFloat("mass", &data.mass, 0.05f, 0.01f, 100.0f, "%.2f");
	ImGui::Checkbox("isKinematic", &data.isKinematic);
	ImGui::Checkbox("onGround", &data.onGround);
}

void RigidBody::applyForce(glm::vec3 a_force)
{
	data.velocity += a_force / data.mass;
}

void RigidBody::applyForceToActor(RigidBody * a_pActor2, glm::vec3 a_force)
{
	if (!a_pActor2->data.onGround) {
		a_pActor2->applyForce(a_force);
	}
	if (!data.onGround) {
		applyForce(-a_force);
	}
}

glm::vec3 RigidBody::predictPosition(float a_time, float a_angle, float a_speed, glm::vec3 a_gravity)
{
	glm::vec3 result(0.0f);
	// calculate velocity
	glm::vec3 velocity(a_speed * cos(a_angle), a_speed * sin(a_angle), 0.0f);
	//
	result = predictPosition(a_time, velocity, a_gravity);

	return result;
}

glm::vec3 RigidBody::predictPosition(float a_time, glm::vec3 a_velocity, glm::vec3 a_gravity)
{
	glm::vec3 result(0.0f);

	result.x = (data.startPosition.x + (a_velocity.x * a_time));
	result.y = (data.startPosition.y + (a_velocity.y * a_time)) + ((a_gravity.y * 0.5) * (a_time * a_time));

	return result;
}

glm::vec3 RigidBody::predictPosition(float a_time, glm::vec3 a_gravity)
{
	glm::vec3 result(0.0f);

	result.x = (data.startPosition.x + (data.startVelocity.x * a_time));
	result.y = (data.startPosition.y + (data.startVelocity.y * a_time)) + ((a_gravity.y * 0.5) * (a_time * a_time));

	return result;
}
