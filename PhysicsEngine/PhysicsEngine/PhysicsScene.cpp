#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"

#include <string>
#include <iostream>

#include "imgui_glfw3.h"
#include <glm\gtx\range.hpp>
#include <glm\gtc\type_ptr.hpp> // used for gl::vec to [] in gui functions


typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFuncs[] = {
	PhysicsScene::planeToPlane, PhysicsScene::planeToSphere,
	PhysicsScene::sphereToPlane, PhysicsScene::sphereToSphere
};

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

void PhysicsScene::checkCollisions()
{
	int actorCount = numberOfActors();

	// check for collisions with this object
	for (int outer = 0; outer < actorCount - 1; outer++) {
		for (int inner = outer +1; inner < actorCount; inner++) {
			// object data
			PhysicsObject* objA = m_actors[outer];
			PhysicsObject* objB = m_actors[inner];
			int shapeIdA = objA->getShapeID();
			int shapeIdB = objB->getShapeID();
			// function pointers
			int functionID = (shapeIdA * SHAPE_COUNT) + shapeIdB;
			fn collisionFuncPtr = collisionFuncs[functionID];
			if (collisionFuncPtr != nullptr) {
				// check for collision
				collisionFuncPtr(objA, objB);
			}
		}
	}
}

bool PhysicsScene::planeToPlane(PhysicsObject * a_planeA, PhysicsObject * a_planeB)
{
	Plane * planeA = (Plane*)a_planeA;
	Plane * planeB = (Plane*)a_planeB;
	// check if objects aren't null before testing
	if (planeA != nullptr && planeB != nullptr) {
		// check plane for collision
		// if the normals are pointing in the same duirection
		//	- just check distance between and distance fom origin
		// if the normal are at an angle
		//	- could do some fancy trig or aabb stuff
	}
	return false;
}

bool PhysicsScene::planeToSphere(PhysicsObject * a_plane, PhysicsObject * a_sphere)
{
	return sphereToPlane(a_sphere, a_plane);
}

bool PhysicsScene::sphereToSphere(PhysicsObject * a_sphereA, PhysicsObject * a_sphereB)
{
	Sphere * sphereA = (Sphere*)a_sphereA;
	Sphere * sphereB = (Sphere*)a_sphereB;
	// check if objects aren't null before testing
	if (sphereA != nullptr && sphereB != nullptr) {
		// check sphere for collision
		float distance = glm::distance(sphereA->getPosition(), sphereB->getPosition());
		float totalRadius = sphereA->getRadius() + sphereA->getRadius();
		// compare distance between centers to combined radius
		if (distance < totalRadius) {
			// object colliding yes, stop objects
			sphereA->setVelocity(glm::vec3(0.0f));
			sphereB->setVelocity(glm::vec3(0.0f));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphereToPlane(PhysicsObject * a_sphere, PhysicsObject * a_plane)
{
	Sphere * sphere = (Sphere*)a_sphere;
	Plane * plane = (Plane*)a_plane;
	// check if objects aren't null before testing
	if (sphere != nullptr && plane != nullptr) {
		
		glm::vec3 planeNorm = plane->getNormal();

		// drop a line from sphere to plane
		glm::vec3 centerPoint(planeNorm * sphere->getPosition());
		std::cout << " Center point = " << centerPoint.x << ", " << centerPoint.y << std::endl;
		glm::vec3 parallel(centerPoint.y, -centerPoint.x, 0.0f);
		std::cout << " Parallel point = " << parallel.x << ", " << parallel.y << std::endl;
		glm::vec3 collisionPoint = planeNorm + parallel;

		float cpDistance = glm::distance(sphere->getPosition(), collisionPoint);
		std::cout << " Collision point = " << collisionPoint.x << ", " << collisionPoint.y << std::endl;
		std::cout << " Collision point distance = " << cpDistance << std::endl;
		//float gapToCenter = glm::distance(sphere->getPosition(), collisionPoint);
		//std::cout << " Plane center distance = " << gapToCenter << std::endl;

		if (cpDistance < sphere->getRadius()) {
			// object colliding, stop object
			sphere->setVelocity(glm::vec3(0.0f));
			return true;
		}
	}
	return false;
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
	ImGui::DragFloat3("gravity", glm::value_ptr(m_gravity), 0.05f, -100.0f, 100.0f, "%.2f");
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
