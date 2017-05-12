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
	applyForce(gravity * data.mass * timeStep);
	data.position += data.velocity * timeStep;
}

void RigidBody::debug()
{	// create agent window
	std::string windowName = "RigidBody";
	ImGui::Text(windowName.c_str());
	ImGui::InputFloat3("position", glm::value_ptr(data.position), 2);
	ImGui::InputFloat3("velocity", glm::value_ptr(data.velocity), 2);
	ImGui::InputFloat3("rotation", glm::value_ptr(data.rotation), 2);
	ImGui::InputFloat("mass", &data.mass, 0.05f, 1.0f, 2);
}

void RigidBody::applyForce(glm::vec3 a_force)
{
	data.velocity += a_force / data.mass;
}

void RigidBody::applyForceToActor(RigidBody * a_pActor2, glm::vec3 a_force)
{
	a_pActor2->applyForce(a_force);
	applyForce(-a_force);
}
