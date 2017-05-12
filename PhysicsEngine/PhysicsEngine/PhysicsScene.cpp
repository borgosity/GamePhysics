#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "imgui_glfw3.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>

PhysicsScene::PhysicsScene()
{
	// gui controls
	m_applyForce = false;
	m_iActorA = 0;
	m_iActorB = 1;

}


PhysicsScene::~PhysicsScene()
{
	for (auto actor : m_actors)
	{
		delete actor;
	}
}

void PhysicsScene::addActor(PhysicsObject * a_actor)
{
	m_actors.push_back(a_actor);
}

void PhysicsScene::removeActor(PhysicsObject * a_actor)
{
	// create an iterator to the actor in the vector
	auto actorItr = std::find(m_actors.begin(), m_actors.end(), a_actor);
	// check iterator is in vector scope befor erasing
	if (actorItr < m_actors.end()){
		m_actors.erase(actorItr);
	}
}

void PhysicsScene::resetScene()
{
	// reset all actor positiona nd velocity
	for (auto actor : m_actors) {
		actor->resetPosition();
		actor->resetVelocity();
	}
	// turn off applyForce if on
	if (m_applyForce){
		m_applyForce = false;
	}
}

void PhysicsScene::clearScene()
{
	m_actors.clear();
}

void PhysicsScene::update(float a_dt)
{
	// update physics at fixed time step
	static float timer = 0;
	timer += a_dt;
	if (timer >= m_timeStep) {
		timer -= m_timeStep;
		// update all
		for (auto actor : m_actors) {
			actor->fixedUpdate(m_gravity, a_dt);
		}
		// if apply for was selected
		if (m_applyForce) {
			// find actors
			if (m_pActorA == nullptr || m_pActorB == nullptr) {
				int count = 0;
				for (auto actor : m_actors) {
					if (count == m_iActorA) {
						m_pActorA = actor;
					}
					if (count == m_iActorB) {
						m_pActorB = actor;
					}
					// increment count
					count++;
				}
			}
			// add force
			m_pActorA->rigidbody()->applyForceToActor(m_pActorB->rigidbody(),glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else {
			// set actors back to nullptr
			m_pActorA = nullptr; 
			m_pActorB = nullptr;
		}
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto actor : m_actors) {
		actor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	ImGui::Begin("Debug");
	std::string text = "Physics Scene";
	ImGui::Text(text.c_str());
	ImGui::InputFloat3("gravity", glm::value_ptr(m_gravity), 2);
	// apply force options
	ImGui::InputInt("Actor A", &m_iActorA, 1, 1);
	ImGui::InputInt("Actor B", &m_iActorB, 1, 1);
	ImGui::PushID(5);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(5 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(5 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(5 / 7.0f, 0.8f, 0.8f));
	// draw button and detect click
	if (ImGui::Button("Force")) m_applyForce = !m_applyForce;
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// draw actor data
	for (auto actor : m_actors) {
		ImGui::PushID(count);
		ImGui::Separator();
		text = "Actor: " + std::to_string(count);
		ImGui::Text(text.c_str());
		actor->debug();
		ImGui::PopID();
		count++;
	}
	ImGui::End();
}
