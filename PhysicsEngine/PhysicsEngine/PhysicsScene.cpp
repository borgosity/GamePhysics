#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "imgui_glfw3.h"

PhysicsScene::PhysicsScene()
{
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

void PhysicsScene::update(float a_dt)
{
	// update physics at fixed time step
	static float timer = 0;
	timer += a_dt;
	if (timer >= m_timeStep) {
		timer -= m_timeStep;

		for (auto actor : m_actors) {
			actor->fixedUpdate(m_gravity,a_dt);
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
	std::string text = "actor-" + count;
	for (auto actor : m_actors) {
		actor->debug();
		ImGui::Separator();
		ImGui::Text(text.c_str());
		count++;
	}
}
