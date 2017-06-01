#include "SpringJoint.h"
#include "RigidBody.h"
#include "Gizmos.h"

#include "imgui_glfw3.h"

SpringJoint::SpringJoint(RigidBody * connectionA, RigidBody * connectionB, float spring, float damping, bool twoD)
{
	m_connections[0] = connectionA;
	m_connections[1] = connectionB;
	m_spring = spring;
	m_damping = damping;
	m_restLength = glm::length(m_connections[0]->data.position - m_connections[1]->data.position);
	m_2D = twoD;
}

SpringJoint::~SpringJoint()
{
}

void SpringJoint::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	// hooke law: F = -k*X - bv
	// - F = force exerted
	// - k = spring constant
	// - X = displacement (stretch) 
	// - b = dampening value 
	// - v = relative velocity of object
	float currentLength = glm::length(m_connections[0]->data.position - m_connections[1]->data.position);
	float stretch = currentLength - m_restLength;

	if (abs(stretch) > 0.01f)
	{
		glm::vec3 relativeVelocity = m_connections[0]->data.velocity - m_connections[1]->data.velocity;
		glm::vec3 relativeDistance = m_connections[0]->data.position - m_connections[1]->data.position;

		//glm::vec3 displacement = relativeVelocity + relativeDistance / timeStep;
		glm::vec3 displacement = (relativeVelocity + relativeDistance / timeStep) * stretch; 
		glm::vec3 forceVector = displacement / (m_connections[0]->data.mass + m_connections[1]->data.mass) * m_damping;

		//float someNumber = -m_spring * stretch;
		//glm::vec3 anotherNumber = relativeVelocity * m_damping;
		//glm::vec3 forceVector = someNumber - anotherNumber;

		m_connections[1]->applyForce(forceVector);
	}
}

void SpringJoint::debug()
{
	ImGui::Text(">--- Spring ---<");
	ImGui::DragFloat("spring", &m_spring, 0.5f, 0.01f, 1000.0f, "%.2f");
	ImGui::DragFloat("damping", &m_damping, 0.01f, 0.01f, 1.0f, "%.2f");
	ImGui::DragFloat("restLength", &m_restLength, 0.5f, 0.01f, 100.0f, "%.2f");
}

void SpringJoint::makeGizmo()
{
	// line colour
	glm::vec4 red(1.0f, 0.5f, 0.0f, 1.0f);
	if (m_2D) {
		// convert to 2D
		glm::vec2 start = glm::vec2(m_connections[0]->data.position);
		glm::vec2 end = glm::vec2(m_connections[1]->data.position);
		// draw 2D line
		aie::Gizmos::add2DLine(start, end, red);
	}
	else {
		// draw 3D line
		aie::Gizmos::addLine(m_connections[0]->data.position, m_connections[1]->data.position, red);
	}
}
