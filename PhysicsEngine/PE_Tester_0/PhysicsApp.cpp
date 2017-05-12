#include "PhysicsApp.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"

#include "imgui_glfw3.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\ext.hpp"

#include "PhysicsObject.h"
#include "RigidBody.h"
#include "PhysicsScene.h"
#include "Sphere.h"


PhysicsApp::PhysicsApp()
{
}


PhysicsApp::~PhysicsApp()
{
}

bool PhysicsApp::startup()
{
	setBackgroundColour(0.15f, 0.15f, 0.15f);
	// initialise gizmo primitive counts
	aie::Gizmos::create(10000, 10000, 10000, 10000);
	// create simple camera transforms
	m_viewMatrix = glm::lookAt(glm::vec3(10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
											getWindowWidth() / (float)getWindowHeight(),
											0.1f, 1000.f);

	// renderer
	m_renderChosen = false;
	m_render3D = false;
	m_render2D = false;
	m_debug = false;
	m_renderer = new aie::Renderer2D();
	m_font = new aie::Font("./../font/consolas.ttf", 32);

	// physics
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec3(0.0f, 0.0f, 0.0f));
	m_physicsScene->setTimeStep(0.01f);

	// test 

	return true;
}

void PhysicsApp::shutdown()
{
	delete m_font;
	delete m_renderer;
	delete m_poNumberOne;
	delete m_physicsScene;
	aie::Gizmos::destroy();
}

void PhysicsApp::fixedUpdate(float a_dt)
{
	if (a_dt <= 0.0f) {
		return;
	}
	m_physicsScene->update(a_dt);
	m_physicsScene->updateGizmos();
}

void PhysicsApp::update2D(float a_dt)
{
}

void PhysicsApp::update3D(float a_dt)
{
}

void PhysicsApp::update(float a_dt)
{
	// update input
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	if (m_renderChosen) {
		fixedUpdate(a_dt);
	}
	// select a demo to display
	switch (m_demo)
	{
	case DEMO1:
		demo1();
		break;
	case DEMO2:
		demo2();
		break;
	case DEMO3:
		demo3(a_dt);
		break;
	case DEMO4:
		break;
	case DEMO5:
		break;
	default:
		break;
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_renderer->begin();

	// ----------------------      draw your stuff here!         ----------------------
	// draw 2D if requested
	if (m_render2D) {
		draw2D();
	}
	// draw 3D if requested
	if (m_render3D) {
		draw3D();
	}
	drawGUI();
	// draw debug if requested
	if (m_debug) {
		m_physicsScene->debugScene();
	}
	// output some text
	m_renderer->drawText(m_font, "Press ESC to quit", HALF_SW, HALF_SH);

	// done drawing sprites
	m_renderer->end();
}

void PhysicsApp::drawGUI()
{
	// Draw agent GUI window
	std::string windowName = "Render Options";
	ImGui::Begin(windowName.c_str());
	//------------------------------------ demo buttons
	ImGui::Text("Select Demo:");
	static int demoMode = DEMO1;
	ImGui::RadioButton("Demo 1", &demoMode, DEMO1);
	ImGui::SameLine();
	ImGui::RadioButton("Demo 2", &demoMode, DEMO2);
	ImGui::SameLine();
	ImGui::RadioButton("Demo 3", &demoMode, DEMO3);
	ImGui::SameLine();
	ImGui::RadioButton("Demo 4", &demoMode, DEMO4);
	if (demoMode != m_demo) {
		m_renderChosen = false;
		clear();
	}
	m_demo = DemoType(demoMode);
	ImGui::Separator();
	// ---------------------------------- render options
	ImGui::Text("Select Render Type:");
	// button 1
	ImGui::PushID(1);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
	// draw button and detect click
	if (ImGui::Button("> 2D <")) {
		m_render3D = false;
		m_renderChosen = false;
		clear();
		m_render2D = true;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// button 2
	ImGui::PushID(2);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
	ImGui::SameLine();
	// draw button and detect click
	if (ImGui::Button("> 3D <")) {
		m_render2D = false;
		m_renderChosen = false;
		clear();
		m_render3D = true;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	ImGui::Separator();
	// --------------------------------------- Debug
	// button 3
	ImGui::PushID(3);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
	// draw button and detect click
	if (ImGui::Button("Debug")) m_debug = !m_debug;
	ImGui::SameLine();
	ImGui::Text("Toggle Debug"); 
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// button 4
	ImGui::PushID(4);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
	// draw button and detect click
	if (ImGui::Button("Reset")) {
		reset();
	};
	ImGui::SameLine();
	ImGui::Text("Reset Demo");
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// button 5
	ImGui::PushID(5);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(5 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(5 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(5 / 7.0f, 0.8f, 0.8f));
	// draw button and detect click
	if (ImGui::Button("Clear")) {
		m_render2D = false;
		m_render3D = false;
		m_renderChosen = false;
		clear();
	};
	ImGui::SameLine();
	ImGui::Text("Clear Scene");
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	ImGui::End();
}

void PhysicsApp::draw2D()
{
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(
		-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f
		)
	);
	m_renderer->drawLine(HALF_SW, 0.0f, HALF_SW, SCREEN_H, 1.0f);
	m_renderer->drawLine(0.0f, HALF_SH, SCREEN_W, HALF_SH, 1.0f);
}

void PhysicsApp::draw3D()
{
	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	// draw a simple grid with gizmos
	glm::vec4 white(1.0f);
	glm::vec4 grey(0.5f, 0.5f, 0.5f, 1.0f);
	for (int i = 0; i < 21; ++i) {
		aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : grey);
		aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? white : grey);
	}

	// draw gizmos
	aie::Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

void PhysicsApp::reset()
{
	m_physicsScene->resetScene();
}

void PhysicsApp::clear()
{
	m_physicsScene->clearScene();
}

void PhysicsApp::demo1()
{
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			Sphere * ball = new Sphere(glm::vec3(-15.0f, 5.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), 3.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(ball);
			m_renderChosen = true;
		}
	}
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen) {
			Sphere * ball = new Sphere(glm::vec3(-11.0f, 0.0f, 0.01f), glm::vec3(5.0f, 0.0f, 0.0f), 3.0f, 0.5f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(ball);
			m_renderChosen = true;
		}
	}
}
// force balls
void PhysicsApp::demo2()
{
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			Sphere * ballA = new Sphere(glm::vec3(-5.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(ballA);
			Sphere * ballB = new Sphere(glm::vec3(5.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(ballB);
			m_renderChosen = true;
		}
	}
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen) {
			Sphere * ballA = new Sphere(glm::vec3(-0.5f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 0.5f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(ballA);
			Sphere * ballB = new Sphere(glm::vec3(0.5f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 0.5f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(ballB);
			m_renderChosen = true;
		}
	}
}

void PhysicsApp::demo3(float a_dt)
{
	// 2D simulation
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			m_rocket = new Sphere(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, 2.0f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(m_rocket);
			m_renderChosen = true;
		}
		// rocket mass
		if (m_rocket->rigidbody()->data.mass > 3.0f) {

			// reduce mass
			if (m_burnTime < 0.0f) {
				m_burnTime = 0.5f;

				m_rocket->rigidbody()->data.mass -= 0.5f;
				glm::vec3 postion = m_rocket->getPosition() + glm::vec3(0.0f, -m_rocket->getRadius(), 0.0f);
				Sphere * gas = new Sphere(glm::vec3(postion.x, postion.y + 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.5f, glm::vec4(0.5, 0.5, 0.5, 1));
				m_rocket->rigidbody()->applyForceToActor(gas->rigidbody(), glm::vec3(0.0f, -2.0f, 0.0f));
				m_physicsScene->addActor(gas);
			}
			m_burnTime -= a_dt;
		}
	}
	// 3D simulation
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen) {
			m_rocket = new Sphere(glm::vec3(-0.0f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, 0.5f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(m_rocket);
			m_renderChosen = true;
			m_burnTime = 0;
		}
		// rocket mass
		if (m_rocket->rigidbody()->data.mass > 3.0f) {
			
			// reduce mass
			if (m_burnTime < 0.0f) {
				m_burnTime = 0.5f;

				m_rocket->rigidbody()->data.mass -= 0.5f;
				glm::vec3 postion = m_rocket->getPosition() + glm::vec3(0.0f, -m_rocket->getRadius(), 0.0f);
				Sphere * gas = new Sphere(glm::vec3(postion.x, postion.y + 0.5f, postion.z), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.2f, glm::vec4(0.5, 0.5, 0.5, 1));
				m_rocket->rigidbody()->applyForceToActor(gas->rigidbody(), glm::vec3(0.0f, -2.0f, 0.0f));
				m_physicsScene->addActor(gas);
			}
			m_burnTime -= a_dt;
		}
	}
}
