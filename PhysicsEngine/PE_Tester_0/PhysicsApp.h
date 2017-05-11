#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Renderer2D.h"

#include "GameDef.h"

class PhysicsObject;
class PhysicsScene;

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
};

