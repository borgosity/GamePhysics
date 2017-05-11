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
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
}

RigidBody::RigidBody(glm::vec3 position, float angle, float speed, glm::vec3 rotation, float mass)
{
	m_position = position;
	m_velocity = glm::vec3(speed * cos(angle), speed * sin(angle), 0.0f);
	m_rotation = rotation;
	m_mass = mass;
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void RigidBody::debug()
{	// create agent window
	std::string windowName = "RigidBody";
	ImGui::Begin(windowName.c_str());
	ImGui::InputFloat3("position", glm::value_ptr(m_position), 2);
	ImGui::InputFloat3("velocity", glm::value_ptr(m_velocity), 2);
	ImGui::InputFloat3("rotation", glm::value_ptr(m_rotation), 2);
	ImGui::InputFloat("mass", &m_mass, 0.05f, 1.0f, 2);
	ImGui::End();
}

void RigidBody::applyForce(glm::vec3 a_force)
{
	m_velocity += a_force / m_mass;
}

void RigidBody::applYForceToActor(RigidBody * a_pActor2, glm::vec3 a_force)
{
	a_pActor2->applyForce(a_force);
	applyForce(-a_force);
}
