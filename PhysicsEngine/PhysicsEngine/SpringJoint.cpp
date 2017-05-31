#include "SpringJoint.h"
#include "RigidBody.h"
#include "Gizmos.h"

#include "imgui_glfw3.h"

SpringJoint::SpringJoint(RigidBody * connectionA, RigidBody * connectionB, float spring, float damping)
{
	m_connections[0] = connectionA;
	m_connections[1] = connectionB;
	m_spring = spring;
	m_damping = damping;
	m_restLength = glm::length(m_connections[0]->data.position - m_connections[1]->data.position);
}

SpringJoint::~SpringJoint()
{
}

void SpringJoint::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	float currentLength = glm::length(m_connections[0]->data.position - m_connections[1]->data.position);
	glm::vec3 relativeDistance = m_connections[0]->data.position - m_connections[1]->data.position;
	glm::vec3 relativeVelocity = m_connections[0]->data.velocity - m_connections[1]->data.velocity;
	float displacement = currentLength - m_restLength;

	glm::vec3 remove = relativeVelocity + relativeDistance / timeStep;
	glm::vec3 forceVector = remove / (m_connections[0]->data.mass + m_connections[0]->data.mass);

	float force = -m_spring * displacement;
	//float relativeVelocity = m_connections[1]->data.velocity
}

void SpringJoint::debug()
{
	ImGui::Text(">--- Spring Start... ");
	m_connections[0]->debug();
	m_connections[1]->debug();
	ImGui::Text("... Spring End ---< ");
}

void SpringJoint::makeGizmo()
{
	// line colour
	glm::vec4 blue(0.0f, 0.5f, 1.0f, 1.0f);
	// line horizontal
	aie::Gizmos::addLine(m_connections[0]->data.position, m_connections[1]->data.position, blue);
}
