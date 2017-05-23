#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "imgui_glfw3.h"
#include <glm\gtx\range.hpp>
#include <glm\gtc\type_ptr.hpp> // used for gl::vec to [] in gui functions


typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFuncs[] = {
	PhysicsScene::planeToPlane, PhysicsScene::planeToSphere, PhysicsScene::planeToBox,
	PhysicsScene::sphereToPlane, PhysicsScene::sphereToSphere, PhysicsScene::sphereToBox,
	PhysicsScene::boxToPlane, PhysicsScene::boxToSphere, PhysicsScene::boxToBox
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

void PhysicsScene::update(float a_dt)
{
	// update physics at fixed time step
	static float timer = 0;
	timer += a_dt;
	if (timer >= m_timeStep) {
		timer -= m_timeStep;
		// update all
		for (auto actor : m_actors) {
			if (actor != nullptr) {
				actor->fixedUpdate( properties.gravity ? m_gravity : glm::vec3(0.0f), a_dt);
			}
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
		// check for collisions
		if (properties.collisions) {
			checkCollisions();
		}
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto actor : m_actors) {
		actor->makeGizmo();
	}
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
/*********************************************************************************************************
* Plane to Object collsions
**********************************************************************************************************/
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

bool PhysicsScene::planeToBox(PhysicsObject * a_plane, PhysicsObject * a_box)
{
	return boxToPlane(a_box, a_plane);;
}
/*********************************************************************************************************
* Sphere to Object collsions
**********************************************************************************************************/
bool PhysicsScene::sphereToSphere(PhysicsObject * a_sphereA, PhysicsObject * a_sphereB)
{
	Sphere * sphereA = (Sphere*)a_sphereA;
	Sphere * sphereB = (Sphere*)a_sphereB;
	// check if objects aren't null before testing
	if (sphereA != nullptr && sphereB != nullptr) {
		// check sphere for collision
		float distance = glm::distance(sphereA->getPosition(), sphereB->getPosition());
		float totalRadius = sphereA->getRadius() + sphereB->getRadius();
		// compare distance between centers to combined radius
		if (distance < totalRadius) {
			// cahce bool values
			bool kinematicA = sphereA->rigidbody()->data.isKinematic;
			bool kinematicB = sphereB->rigidbody()->data.isKinematic;
			bool onGroundA = sphereA->rigidbody()->data.onGround;
			bool onGroundB = sphereB->rigidbody()->data.onGround;
			// check either is kinematic
			if (kinematicA || kinematicB) {
				// get the normal of the gap between objects
				glm::vec3 collisionNormal = glm::normalize(sphereB->getPosition() - sphereA->getPosition());
				// if both spheres are not on the ground
				if (!onGroundA && !onGroundB) {
					// calculate force vector
					glm::vec3 relativeVelocity = sphereA->getVelocity() - sphereB->getVelocity();
					glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
					glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / sphereA->rigidbody()->data.mass + 1.0f / sphereB->rigidbody()->data.mass);
					// combine elasticity
					float combinedElasticity = (sphereA->rigidbody()->data.elasticity +
												sphereB->rigidbody()->data.elasticity / 2.0f);
					// use Newton's third law to apply collision forces to colliding bodies 
					sphereA->rigidbody()->applyForceToActor(sphereB->rigidbody(), forceVector + (forceVector*combinedElasticity));
					// move out spheres out of collision 
					glm::vec3 separationVector = collisionNormal * distance * 0.5f;
					sphereA->setPosition(sphereA->getPosition() - separationVector);
					sphereB->setPosition(sphereB->getPosition() + separationVector);
				}
				// if one sphere is on the ground treat collsion as plane collision
				if (onGroundA || onGroundB) {
					// determine moving sphere
					Sphere * sphere = (onGroundA ? sphereB : sphereA);
					Sphere * sphereGround = (onGroundA ? sphereA : sphereB);
					// calculate force vector
					glm::vec3 forceVector = -1 * sphere->rigidbody()->data.mass * collisionNormal * (glm::dot(collisionNormal, sphere->getVelocity()));
					// apply force
					sphere->rigidbody()->applyForce(forceVector * 2.0f);
					// move out of collision
					glm::vec3 separationVector = collisionNormal * distance * 0.5f;
					sphere->setPosition(sphere->getPosition() - separationVector);
					// stop other sphere from being on ground
					sphereGround->rigidbody()->data.onGround = false;
				}
			}
			else {
				// object colliding yes, stop objects
				sphereA->setVelocity(glm::vec3(0.0f));
				sphereB->setVelocity(glm::vec3(0.0f));
				if (onGroundA || onGroundB) {
					sphereA->rigidbody()->data.onGround = true;
					sphereB->rigidbody()->data.onGround = true;
				}
			}
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
		bool kinematic = sphere->rigidbody()->data.isKinematic;
		glm::vec3 planeNorm = plane->getNormal();
		float planeDO = plane->getDistance();
		// magnitude of sphere vector, plane normal
		float mag = (dot(sphere->getPosition(), planeNorm));

		// if planeNorm is below 0 magnitude will be negative
		if (mag < 0)
		{
			planeNorm *= -1;
			mag *= -1;
		}

		float collision = mag - sphere->getRadius();
		// collision check
		if (collision < 0.0f) {
			if (kinematic) {
				// calculate force vector
				glm::vec3 forceVector = -1 * sphere->rigidbody()->data.mass * planeNorm * (glm::dot(planeNorm, sphere->getVelocity()));
				// combine elasticity
				float combinedElasticity = (sphere->rigidbody()->data.elasticity +
											plane->getElasticity() / 2.0f);
				// only bounce if not resting on the ground
				if (!sphere->rigidbody()->data.onGround) {
					sphere->rigidbody()->applyForce(forceVector + (forceVector*combinedElasticity));
					// move out of collision
					glm::vec3 separationVector = planeNorm * collision * 0.5f;
					sphere->setPosition(sphere->getPosition() - separationVector);
				}
			}
			else {
				// object colliding, stop object
				sphere->setVelocity(glm::vec3(0.0f));
				sphere->rigidbody()->data.onGround = true;
			}
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphereToBox(PhysicsObject * a_sphere, PhysicsObject * a_box)
{
	return boxToSphere(a_box, a_sphere);
}
/*********************************************************************************************************
* Box to Object collsions
**********************************************************************************************************/
bool PhysicsScene::boxToSphere(PhysicsObject * a_box, PhysicsObject * a_sphere)
{
	Box * box = (Box*)a_box;
	Sphere * sphere = (Sphere*)a_sphere;
	// check if objects aren't null before testing
	if (box != nullptr && sphere != nullptr) {
		// collision check
		if (box->testAABB(sphere)) {
			// cache some bools for later use
			bool kinematicA = box->rigidbody()->data.isKinematic;
			bool kinematicB = sphere->rigidbody()->data.isKinematic;
			bool onGroundA = box->rigidbody()->data.onGround;
			bool onGroundB = sphere->rigidbody()->data.onGround;
			// check either is kinematic
			if (kinematicA || kinematicB) {
				glm::vec3 centerDist = sphere->getPosition() - box->getPosition();
				glm::vec3 boxesMaxSize = glm::vec3(box->getSize() + sphere->getRadius());
				glm::vec3 collisionNormal = glm::normalize(centerDist);
				glm::vec3 overlap = abs(centerDist - boxesMaxSize);
				// if both boxs are not on the ground
				if (!onGroundA && !onGroundB) {
					// calculate force vector
					glm::vec3 relativeVelocity = box->getVelocity() - sphere->getVelocity();
					glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
					glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / box->rigidbody()->data.mass + 1.0f / sphere->rigidbody()->data.mass);
					// combine elasticity
					float combinedElasticity = (box->rigidbody()->data.elasticity +
												sphere->rigidbody()->data.elasticity / 2.0f);
					// use Newton's third law to apply collision forces to colliding bodies 
					box->rigidbody()->applyForceToActor(sphere->rigidbody(), forceVector + (forceVector*combinedElasticity));
					// move out boxs out of collision 
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					box->setPosition(box->getPosition() - separationVector);
					sphere->setPosition(sphere->getPosition() + separationVector);
				}
				// if one box is on the ground treat collsion as plane collision
				if (onGroundA || onGroundB) {
					// determine moving box
					PhysicsObject * obj = (onGroundA ? (PhysicsObject*)sphere : (PhysicsObject*)box);
					PhysicsObject * objGround = (onGroundA ? (PhysicsObject*)box : (PhysicsObject*)sphere);
					// calculate force vector
					glm::vec3 forceVector = -1 * obj->rigidbody()->data.mass * collisionNormal * (glm::dot(collisionNormal, obj->getVelocity()));
					// apply force
					obj->rigidbody()->applyForce(forceVector * 2.0f);
					// move out of collision
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					obj->setPosition(obj->getPosition() - separationVector);
					// stop other box from being on ground
					objGround->rigidbody()->data.onGround = false;
				}
			}
			else {
				// object colliding yes, stop objects
				box->setVelocity(glm::vec3(0.0f));
				sphere->setVelocity(glm::vec3(0.0f));
				if (onGroundA || onGroundB) {
					box->rigidbody()->data.onGround = true;
					sphere->rigidbody()->data.onGround = true;
				}
			}
			return true;
		}
	}
	return false;
}

bool PhysicsScene::boxToPlane(PhysicsObject * a_box, PhysicsObject * a_plane)
{
	Box * box = (Box*)a_box;
	Plane * plane = (Plane*)a_plane;
	// check if objects aren't null before testing
	if (box != nullptr && plane != nullptr) {
		glm::vec3 planeNormal = plane->getNormal();
		glm::vec3 center = box->getPosition();
		glm::vec3 extents(box->getSize());
		// magnitude of box center and plane vectors
		float mag = dot(planeNormal, center);
		// projection interval radius of box onto the plane
		float radius = extents.x * abs(planeNormal.x) + extents.y * abs(planeNormal.y) + extents.z * abs(planeNormal.z);

		// if planeNorm is below 0 magnitude will be negative
		if (mag < 0)
		{
			planeNormal *= -1;
			mag *= -1;
		}

		float collision = mag - radius;

		// collision check
		if (collision <= 0.0f) {
			// cache some data
			bool kinematic = box->rigidbody()->data.isKinematic;
			if (kinematic) {
				// calculate force vector
				glm::vec3 forceVector = -1 * box->rigidbody()->data.mass * planeNormal * (glm::dot(planeNormal, box->getVelocity()));
				// combine elasticity
				float combinedElasticity = (box->rigidbody()->data.elasticity +
											plane->getElasticity() / 2.0f);
				// only bounce if not resting on the ground
				if (!box->rigidbody()->data.onGround) {
					box->rigidbody()->applyForce(forceVector + (forceVector*combinedElasticity));
					// move out of collision
					glm::vec3 separationVector = planeNormal * collision * 0.5f;
					box->setPosition(box->getPosition() - separationVector);
				}
			}
			else {
				// object colliding, stop object
				box->setVelocity(glm::vec3(0.0f));
				box->rigidbody()->data.onGround = true;
			}
			return true;
		}
	}
	return false;
}

bool PhysicsScene::boxToBox(PhysicsObject * a_boxA, PhysicsObject * a_boxB)
{
	Box * boxA = (Box*)a_boxA;
	Box * boxB = (Box*)a_boxB;
	// check if objects aren't null before testing
	if (boxA != nullptr && boxB != nullptr) {

		// collision check
		if (boxA->testAABB(boxB)) {
			// cache some bools for later use
			bool kinematicA = boxA->rigidbody()->data.isKinematic;
			bool kinematicB = boxB->rigidbody()->data.isKinematic;
			bool onGroundA = boxA->rigidbody()->data.onGround;
			bool onGroundB = boxB->rigidbody()->data.onGround;
			// check either is kinematic
			if (kinematicA || kinematicB) {
				glm::vec3 centerDist = boxB->getPosition() - boxA->getPosition();
				glm::vec3 boxesMaxSize = glm::vec3(boxA->getSize() + boxB->getSize());
				glm::vec3 collisionNormal = glm::normalize(centerDist);
				glm::vec3 overlap = abs(centerDist - boxesMaxSize);
				// if both boxs are not on the ground
				if (!onGroundA && !onGroundB) {
					// calculate force vector
					glm::vec3 relativeVelocity = boxA->getVelocity() - boxB->getVelocity();
					glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
					glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / boxA->rigidbody()->data.mass + 1.0f / boxB->rigidbody()->data.mass);
					// combine elasticity
					float combinedElasticity = (boxA->rigidbody()->data.elasticity +
												boxB->rigidbody()->data.elasticity / 2.0f);
					// use Newton's third law to apply collision forces to colliding bodies 
					boxA->rigidbody()->applyForceToActor(boxB->rigidbody(), forceVector + (forceVector*combinedElasticity));
					// move out boxs out of collision 
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					boxA->setPosition(boxA->getPosition() - separationVector);
					boxB->setPosition(boxB->getPosition() + separationVector);
				}
				// if one box is on the ground treat collsion as plane collision
				if (onGroundA || onGroundB) {
					// determine moving box
					Box * box = (onGroundA ? boxB : boxA);
					Box * boxGround = (onGroundA ? boxA : boxB);
					// calculate force vector
					glm::vec3 forceVector = -1 * box->rigidbody()->data.mass * collisionNormal * (glm::dot(collisionNormal, box->getVelocity()));
					// apply force
					box->rigidbody()->applyForce(forceVector * 2.0f);
					// move out of collision
					glm::vec3 separationVector = collisionNormal * overlap * 0.5f;
					box->setPosition(box->getPosition() - separationVector);
					// stop other box from being on ground
					boxGround->rigidbody()->data.onGround = false;
				}
			}
			else {
				// object colliding yes, stop objects
				boxA->setVelocity(glm::vec3(0.0f));
				boxB->setVelocity(glm::vec3(0.0f));
				if (onGroundA || onGroundB) {
					boxA->rigidbody()->data.onGround = true;
					boxB->rigidbody()->data.onGround = true;
				}
			}
			return true;
		}
	}
	return false;
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
