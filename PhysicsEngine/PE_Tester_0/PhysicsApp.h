#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Renderer2D.h"

#include "GameDef.h"

class PhysicsObject;
class PhysicsScene;
class Sphere;

enum DemoType {
	DEMO1 = 1,
	DEMO2 = 2,
	DEMO3 = 3,
	DEMO4 = 4,
	DEMO5 = 5
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
	glm::mat4	m_projectionMatrix;

	// demos
	DemoType m_demo;
	void demo1();
	// add force demo
	void demo2();
	// rocket demo
	Sphere * m_rocket = nullptr;
	float m_burnTime;
	void demo3(float a_dt);


	void demo4();
};

