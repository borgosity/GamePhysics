#pragma once
#include "Application.h"
#include <iostream>
#include <glm/glm.hpp>
#include "Renderer2D.h"

#include "GameDef.h"

class PhysicsObject;
class PhysicsScene;
class Sphere;
class Plane;
class Box;

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

	// mouse input
	glm::ivec2 m_firstClick;
	glm::ivec2 m_mouseScreenPos;
	glm::vec2 m_mouseWorldPos;

	bool m_mouseClicked;

	// rendering
	bool m_renderChosen;
	bool m_render2D;
	bool m_render3D;
	bool m_debug;
	aie::Renderer2D * m_renderer;
	aie::Font		* m_fontFooter;
	aie::Font		* m_fontTitle;

	glm::mat4	m_viewMatrix;
	glm::vec3	m_cameraView;
	float		m_2dView;
	glm::mat4	m_projectionMatrix;

	// demos
	DemoType m_demo;
	std::string m_demoName;
	// -- demo 1 --
	void demo1();
	// -- demo 2 --
	// add force demo
	bool m_applyForce;
	glm::vec3 m_forceVector;
	Sphere * m_ballA = nullptr;
	Sphere * m_ballB = nullptr;
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
	Box * m_boxA = nullptr;
	Box * m_boxB = nullptr;
	Box * m_boxC = nullptr;
	Plane * m_planeA = nullptr;
	Plane * m_planeB = nullptr;
	void demo5(float a_dt);
	// -- demo 6 --
	// collision response
	void demo6(float a_dt);
	// -- demo 7 --
	// drag
	void demo7(float a_dt);
	// -- demo 8 --
	// spring
	void demo8(float a_dt);
};

