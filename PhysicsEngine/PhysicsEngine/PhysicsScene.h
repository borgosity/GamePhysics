#pragma once
#include <vector>
#include "glm\glm.hpp"

struct PhysicsProperties {
	bool gravity = false;
	bool collisions = false;
	bool collisionResponse = false;
};

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();
	// update functions
	void update(float a_dt);
	void updateGizmos();
	void debugScene();
	// actor functions
	void addActor(PhysicsObject * a_actor);
	void removeActor(PhysicsObject * a_actor);
	int numberOfActors() { return m_actors.size(); }
	void resetScene();
	void clearScene();
	// get/set
	void setGravity(const glm::vec3 a_gravity) { m_gravity = a_gravity; }
	glm::vec3 setGravity() const { return m_gravity; }
	void setTimeStep(const float a_timeStep) { m_timeStep = a_timeStep; }
	float setTimeStep() const { return m_timeStep; }
	std::vector<PhysicsObject*> actors() { return m_actors; };
	// collision functions
	void checkCollisions();
	// collision detection
	// plane
	static bool planeToPlane(PhysicsObject * a_planeA, PhysicsObject * a_planeB);
	static bool planeToSphere(PhysicsObject *  a_plane, PhysicsObject * a_sphere);
	static bool planeToBox(PhysicsObject *  a_plane, PhysicsObject * a_box);
	// sphere
	static bool sphereToSphere(PhysicsObject * a_sphereA, PhysicsObject * a_sphereB);
	static bool sphereToPlane(PhysicsObject * a_sphere, PhysicsObject * a_plane);
	static bool sphereToBox(PhysicsObject * a_sphere, PhysicsObject * a_box);
	// box
	static bool boxToSphere(PhysicsObject * a_box, PhysicsObject * a_sphere);
	static bool boxToPlane(PhysicsObject * a_box, PhysicsObject * a_plane);
	static bool boxToBox(PhysicsObject * a_boxA, PhysicsObject * a_boxB);

	// scene properties
	PhysicsProperties properties;

protected:
	glm::vec3	m_gravity;
	float		m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	// gui options
	bool m_applyForce;
	int m_iActorA;
	PhysicsObject* m_pActorA = nullptr;
	int m_iActorB;
	PhysicsObject* m_pActorB = nullptr;

};

