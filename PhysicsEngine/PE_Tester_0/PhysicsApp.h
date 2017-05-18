#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include "Renderer2D.h"

#include "GameDef.h"

class PhysicsObject;
class PhysicsScene;
class Sphere;
class Plane;

enum DemoType {
	DEMO1 = 1,
	DEMO2 = 2,
	DEMO3 = 3,
	DEMO4 = 4,
	DEMO5 = 5,
	DEMO6 = 6,
	DEMO7 = 7,
	DEMO8 = 8,
};

class PhysicsApp :
	public aie::Application
{
public:
	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float a_dt);
	void fixedUpdate(float a_dt);
	void update2D(float a_dt);
	void update3D(float a_dt);

	virtual void draw();
	void drawGUI();
	void draw2D();
	void draw3D();

	void reset();
	void clear();

private:
	PhysicsScene * m_physicsScene = nullptr;
	PhysicsObject * m_poNumberOne = nullptr;

	// rendering
	bool m_renderChosen;
	bool m_render2D;
	bool m_render3D;
	bool m_debug;
	aie::Renderer2D * m_renderer;
	aie::Font		* m_font;
	glm::mat4	m_viewMatrix;
	glm::vec3	m_cameraView;
	glm::mat4	m_projectionMatrix;

	// demos
	// -- demo 1 --
	DemoType m_demo;
	void demo1();
	// -- demo 2 --
	// add force demo
	void demo2();
	// -- demo 3 --
	// rocket demo
	Sphere * m_rocket = nullptr;
	float m_burnTime;
	void demo3(float a_dt);
	// -- demo 4 --
	// projectile demo
	PhysicsScene * m_demo4Scene = nullptr;
	Sphere * m_projectile = nullptr;
	float m_timeStep;
	float m_totalTime;
	float m_angle;
	float m_speed;
	void demo4(float a_dt);
	// -- demo 5 --
	// collision detection
	Sphere * m_sphereA = nullptr;
	Sphere * m_sphereB = nullptr;
	Sphere * m_sphereC = nullptr;
	Plane * m_planeA = nullptr;
	Plane * m_planeB = nullptr;
	void demo5(float a_dt);
	// -- demo 6 --
	// collision response
	void demo6(float a_dt);
};

