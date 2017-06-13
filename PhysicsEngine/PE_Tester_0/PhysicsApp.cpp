#include "PhysicsApp.h"
#include "Font.h"
#include "Gizmos.h"
#include <cmath>
#include <iostream>

#include "imgui_glfw3.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\ext.hpp"
#include "glm\gtx\vector_angle.hpp"

#include "PhysicsObject.h"
#include "RigidBody.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "GameDef.h"
#include "SpringJoint.h"

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
	m_2dView = 100.f;
	m_cameraView = glm::vec3(0.0f, 10.0f, 20.0f);
	m_viewMatrix = glm::lookAt(m_cameraView, glm::vec3(0), glm::vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
											getWindowWidth() / (float)getWindowHeight(),
											0.1f, 1000.f);

	// renderer
	m_renderChosen = false;
	m_render3D = false;
	m_render2D = false;
	m_debug = false;
	m_renderer = new aie::Renderer2D();
	std::string path = "../res/font/consolas.ttf";

	m_fontTitle = new aie::Font("../res/font/consolas.ttf", 20);
	m_fontFooter = new aie::Font("../res/font/consolas.ttf", 16);

	//printf(" %s", _fullpath(NULL, path.c_str(), 40));

	// physics
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec3(0.0f, 0.0f, 0.0f));
	m_physicsScene->setTimeStep(0.01f);

	// demo defaults
	m_optionsD8 = false;
	m_spring = 0.5f;
	m_damping = 0.5f;
	m_restLength = 5.0f;

	return true;
}

void PhysicsApp::shutdown()
{
	delete m_fontFooter;
	delete m_fontTitle;
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
	// update demo 4 scene if requested
	if (m_demo == 4) {
		m_demo4Scene->update(a_dt);
		m_demo4Scene->updateGizmos();
	}
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
	// clear all gizmos
	aie::Gizmos::clear();

	if (m_renderChosen && (m_demo != DEMO8)) {
		fixedUpdate(a_dt);
	}
	// select a demo to display
	switch (m_demo)
	{
	case DEMO1:
		m_demoName = "Momentum";
		demo1();
		break;
	case DEMO2:
		m_demoName = "Force";
		demo2();
		break;
	case DEMO3:
		m_demoName = "Rocket Propulsion";
		demo3(a_dt);
		break;
	case DEMO4:
		m_demoName = "Projectile Path";
		demo4(a_dt);
		break;
	case DEMO5:
		m_demoName = "Collision Detection";
		demo5(a_dt);
		break;
	case DEMO6:
		m_demoName = "Collision Response";
		demo6(a_dt);
		break;
	case DEMO7:
		m_demoName = "Drag and Rotation";
		demo7(a_dt);
		break;
	case DEMO8:
		m_demoName = "Joints and Springs";
		demo8(a_dt);
		break;
	case SIM00:
		m_demoName = "Simulation";
		simulation(a_dt, input);
		break;
	default:
		std::cout << "Demo not yet defined" << std::endl;
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
		if (m_demo == 4 && m_renderChosen) {
			m_demo4Scene->debugScene();
		}
	}
	// output some text
	// header text
	m_renderer->drawText(m_fontTitle, m_demoName.c_str(), HALF_SW - ((m_demoName.length() * 0.5f) * 10), SCREEN_H - 20);
	if (m_demo == SIM00) {
		std::string instructions = "Left Click and Drag to Shoot Ball";
		m_renderer->drawText(m_fontTitle, instructions.c_str(), HALF_SW - ((instructions.length() * 0.5f) * 10), SCREEN_H - 40);
	}
	// footer text
	m_renderer->drawText(m_fontFooter, "Press ESC to quit", 0.0f, 0.0f);

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
	ImGui::RadioButton("Demo 5", &demoMode, DEMO5);
	ImGui::SameLine();
	ImGui::RadioButton("Demo 6", &demoMode, DEMO6);
	ImGui::SameLine();
	ImGui::RadioButton("Demo 7", &demoMode, DEMO7);
	ImGui::SameLine();
	ImGui::RadioButton("Demo 8", &demoMode, DEMO8);
	ImGui::RadioButton("Simulation", &demoMode, SIM00);

	if (demoMode != m_demo) {
		m_renderChosen = false;
		m_optionsD8 = false;
		clear();
	}
	m_demo = DemoType(demoMode);
	// do stuff if demo 2 selected
	if (m_demo == DEMO2 && m_renderChosen)
	{
		ImGui::Begin("Force Demo");
		std::string text = "Set a force Vector and click Apply";
		ImGui::Text(text.c_str());
		// apply force options
		ImGui::DragFloat3("Force Vector", glm::value_ptr(m_forceVector), 0.05f, -100.0f, 100.0f, "%.2f");
		ImGui::PushID(5);
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(5 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(5 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(5 / 7.0f, 0.8f, 0.8f));
		// draw button and detect click
		if (ImGui::Button("Apply")) m_applyForce = !m_applyForce;
		ImGui::PopStyleColor(3);
		ImGui::PopID();
		ImGui::End();
	}
	// do stuff if demo 8 selected
	if (m_demo == DEMO8 && !m_optionsD8 && (m_render2D || m_render3D))
	{
		ImGui::Begin("Spring Demo");
		std::string text = "Adjust Spring Values and click Apply";
		ImGui::Text(text.c_str());
		// apply force options
		ImGui::DragFloat("spring", &m_spring, 0.5f, 0.01f, 1000.0f, "%.2f");
		ImGui::DragFloat("damping", &m_damping, 0.01f, 0.01f, 1.0f, "%.2f");
		ImGui::DragFloat("restLength", &m_restLength, 0.5f, 0.01f, 100.0f, "%.2f");
		ImGui::PushID(5);
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(5 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(5 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(5 / 7.0f, 0.8f, 0.8f));
		// draw button and detect click
		if (ImGui::Button("Apply")) {
			m_optionsD8 = !m_optionsD8;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
		ImGui::End();
	}
	// do stuff when SIM00is selected
	if (m_demo == SIM00 && m_renderChosen)
	{
		m_sceneReady = true;
	}
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
		m_optionsD8 = false;
		clear();
		m_render2D = true;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// button 2
	if (m_demo != SIM00) {
		ImGui::PushID(2);
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
		ImGui::SameLine();
		// draw button and detect click
		if (ImGui::Button("> 3D <")) {
			m_render2D = false;
			m_renderChosen = false;
			m_optionsD8 = false;
			clear();
			m_render3D = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
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
	// --------------------------  Camera ---------------------------------
	ImGui::Separator();
	if (m_render3D) {
		ImGui::Text("Adjust Camera");
		ImGui::DragFloat("Left/Right", &m_cameraView.x, 1.0f, 0.01f, 100.0f);
		ImGui::DragFloat("Up/Down", &m_cameraView.y, 1.0f, 0.01f, 100.0f);
		ImGui::DragFloat("In/Out", &m_cameraView.z, 1.0f, 0.01f, 100.0f);
	}
	if (m_render2D) {
		ImGui::Text("Adjust Camera");
		ImGui::DragFloat("Zoom", &m_2dView, 1.0f, 1.0f, 100.0f);
	}
	ImGui::End();
}

void PhysicsApp::draw2D()
{
	// camera view
	static float aspectRatio = 16 / 9.0f;
	float size = m_2dView;

	if (m_demo != SIM00) {
		// line colour
		glm::vec4 grey(0.5f, 0.5f, 0.5f, 1.0f);
		// line horizontal
		glm::vec2 startX(-size, 0.0f );
		glm::vec2 endX(size, 0.0f);
		aie::Gizmos::add2DLine(startX, endX, grey);
		// line vertical
		glm::vec2 startY(0.0f, size / aspectRatio);
		glm::vec2 endY(0.0f, -size / aspectRatio);
		aie::Gizmos::add2DLine(startY, endY, grey);
	}
	else {
		glm::vec4 colour(0.5f, 0.5f, 0.5f, 1.0f);
		aie::Gizmos::add2DLine(cursorTo2dSpace(m_firstClickPos), cursorTo2dSpace(m_mouseScreenPos), colour);
	}
	
	// draw gizmos
	aie::Gizmos::draw2D(glm::ortho<float>(
		-size, size,
		-size / aspectRatio, size / aspectRatio, -1.0f, 1.0f
		)
	);
}

void PhysicsApp::draw3D()
{
	// update view matrix incase view changed
	m_viewMatrix = glm::lookAt(m_cameraView, glm::vec3(0), glm::vec3(0, 1, 0));
	// update perspective in case window resized
	float fovy = glm::pi<float>() * 0.25f;
	float aspect = getWindowWidth() / (float)getWindowHeight();
	m_projectionMatrix = glm::perspective(fovy, aspect,	0.1f, 1000.f);
	// select a demo to display
	switch (m_demo)
	{
	case DEMO5:
		break;
	case DEMO6:
		break;
	case DEMO7:
		break;
	case SIM00:
		break;
	default:
		// draw a simple grid with gizmos
		glm::vec4 white(1.0f);
		glm::vec4 grey(0.5f, 0.5f, 0.5f, 1.0f);
		float size = 20;
		for (int i = 0; i < 41; ++i) {
			// draw on the Z plane, increment along the X
			aie::Gizmos::addLine(glm::vec3(-size + i, 0, size),
				glm::vec3(-size + i, 0, -size),
				i == size ? white : grey);
			// draw on the X plane, increment into the Z
			aie::Gizmos::addLine(glm::vec3(size, 0, -size + i),
				glm::vec3(-size, 0, -size + i),
				i == size ? white : grey);
		}
		break;
	}
	// draw gizmos
	aie::Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}
/// reset scene to start
void PhysicsApp::reset()
{
	m_physicsScene->resetScene();
	if (m_demo4Scene != nullptr) {
		m_demo4Scene->resetScene();
	}
}
/// clear scene of objects
void PhysicsApp::clear()
{
	m_physicsScene->clearScene();
	if (m_demo4Scene != nullptr) {
		m_demo4Scene->clearScene();
	}
}
// translate screen space to 2D world space
glm::vec2 PhysicsApp::cursorTo2dSpace(glm::vec2 a_mouseCoOrd)
{
	static float aspectRatio = 16 / 9.0f;
	float width = (m_2dView * 2.0f) / getWindowWidth();
	float height = ((m_2dView * 2.0f) / aspectRatio) / getWindowHeight();
	glm::vec2 result((a_mouseCoOrd.x * width) - m_2dView, (a_mouseCoOrd.y * height) - (m_2dView / aspectRatio));
	return result;
}
/*****************************************************************************************
*  Demo basic ball movement
*****************************************************************************************/
void PhysicsApp::demo1()
{
	if (!m_renderChosen) {
		if (m_render2D && !m_render3D) {
			Sphere * ball = new Sphere(glm::vec3(-15.0f, 5.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), 3.0f, 1.0f, glm::vec4(0, 1, 0, 1), true);
			m_physicsScene->addActor(ball);
		}
		if (m_render3D && !m_render2D) {
			Sphere * ball = new Sphere(glm::vec3(-11.0f, 0.0f, 0.01f), glm::vec3(5.0f, 0.0f, 0.0f), 3.0f, 0.5f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(ball);
		}
		if (m_render3D || m_render2D) {
			m_physicsScene->setGravity(glm::vec3(0.0f, 0.f, 0.0f));
			m_renderChosen = true;
		}
	}
}
/*****************************************************************************************
*  Demo 2 force ball
*****************************************************************************************/
void PhysicsApp::demo2()
{
	if (!m_renderChosen) {
		if (m_render2D && !m_render3D) {
				Sphere * m_ballA = new Sphere(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 1.0f, glm::vec4(0, 1, 0, 1), true);
				Sphere * m_ballB = new Sphere(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 1.0f, glm::vec4(0, 1, 0, 1), true);
				// add to scene
				m_physicsScene->addActor(m_ballA);
				m_physicsScene->addActor(m_ballB);
		}
		if (m_render3D && !m_render2D) {
				Sphere * m_ballA = new Sphere(glm::vec3(-0.5f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 0.5f, glm::vec4(0, 1, 0, 1));
				Sphere * m_ballB = new Sphere(glm::vec3(0.5f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 0.5f, glm::vec4(0, 1, 0, 1));
				// add to scene
				m_physicsScene->addActor(m_ballA);
				m_physicsScene->addActor(m_ballB);
		}
		if (m_render3D || m_render2D) {
			// set vars
			m_ballA = (Sphere*)m_physicsScene->actors()[0];
			m_ballB = (Sphere*)m_physicsScene->actors()[1];
			m_applyForce = false;
			m_forceVector = glm::vec3(1.0f, 0.0f, 0.0f);
			m_renderChosen = true;
		}

	}
	else
	{
		// apply force if button was clicked
		if (m_applyForce)
		{
			m_ballA->rigidbody()->applyForceToActor(m_ballB->rigidbody(), m_forceVector);
		}
	}
}
/*****************************************************************************************
*  Demo 3 rocket
*****************************************************************************************/
void PhysicsApp::demo3(float a_dt)
{
	// 2D simulation
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			m_rocket = new Sphere(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, 2.0f, glm::vec4(0, 1, 0, 1), true);
			m_rocket->rigidbody()->data.isKinematic = true;
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
				Sphere * gas = new Sphere(glm::vec3(postion.x, postion.y + 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.5f, glm::vec4(0.5, 0.5, 0.5, 1), true);
				gas->rigidbody()->data.isKinematic = true;
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
			m_rocket->rigidbody()->data.isKinematic = true;
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
				gas->rigidbody()->data.isKinematic = true;
				m_rocket->rigidbody()->applyForceToActor(gas->rigidbody(), glm::vec3(0.0f, -2.0f, 0.0f));
				m_physicsScene->addActor(gas);
			}
			m_burnTime -= a_dt;
		}
	}
}
/*****************************************************************************************
*  Demo 4 projectile prediction
*****************************************************************************************/
void PhysicsApp::demo4(float a_dt)
{
	// 2D simulation
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			m_angle = 45.0f;
			m_speed = 25.0f;
			// actual projectile
			m_projectile = new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), m_angle, m_speed, 3.0f, 1.0f, glm::vec4(0, 1, 0, 1), true);
			m_physicsScene->addActor(m_projectile);
			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_renderChosen = true;
			m_timeStep = 0.0f;
			m_totalTime = 0.0f;
			// predictive projectiles
			m_demo4Scene = new 	PhysicsScene();
			m_demo4Scene->setGravity(glm::vec3(0.0f, 0.0f, 0.0f));
			m_demo4Scene->setTimeStep(0.01f);
			// plot initial position
			glm::vec3 position = m_projectile->rigidbody()->predictPosition(m_totalTime, glm::vec3(0.0f, -10.0f, 0.0f));
			// create projectile object
			Sphere * ball = new Sphere(position, glm::vec3(0.0f), 3.0f, 1.0f, glm::vec4(1, 0, 0, 1), true);
			m_demo4Scene->addActor(ball);
		}
		// increase velocity over time
		if (m_timeStep < 0.0f) {
			m_timeStep = 0.25f;
			m_totalTime += 0.1f;
			// get potential future position of object
			glm::vec3 position = m_projectile->rigidbody()->predictPosition(m_totalTime, glm::vec3(0.0f, -10.0f, 0.0f));

			// create projectile object
			if (m_demo4Scene->numberOfActors() < 50) {
				Sphere * ball = new Sphere(position, glm::vec3(0.0f), 3.0f, 1.0f, glm::vec4(1, 0, 0, 1), true);
				m_demo4Scene->addActor(ball);
			}
		}
		m_timeStep -= a_dt;
	}
	// 3D simulation
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen) {
			m_angle = 45.0f;
			m_speed = 10.0f;
			m_projectile = new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), m_angle, m_speed, 3.0f, 0.2f, glm::vec4(0, 1, 0, 1));
			m_physicsScene->addActor(m_projectile);
			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_renderChosen = true;
			m_timeStep = 0.0f;
			m_totalTime = 0.0f;
			// predictive projectiles
			m_demo4Scene = new 	PhysicsScene();
			m_demo4Scene->setGravity(glm::vec3(0.0f, 0.0f, 0.0f));
			m_demo4Scene->setTimeStep(0.01f);
			// plot initial position
			glm::vec3 position = m_projectile->rigidbody()->predictPosition(m_totalTime, glm::vec3(0.0f, -10.0f, 0.0f));
			// create projectile object
			Sphere * ball = new Sphere(position, glm::vec3(0.0f), 3.0f, 0.2f, glm::vec4(1, 0, 0, 1));
			m_demo4Scene->addActor(ball);
		}
		// increase velocity over time
		if (m_timeStep < 0.0f) {
			m_timeStep = 0.25f;
			m_totalTime += 0.1f;
			// get potential future position of object
			glm::vec3 position = m_projectile->rigidbody()->predictPosition(m_totalTime, glm::vec3(0.0f, -10.0f, 0.0f));
			
			// create projectile object
			if (m_demo4Scene->numberOfActors() < 50) {
				Sphere * ball = new Sphere(position, glm::vec3(0.0f), 3.0f, 0.2f, glm::vec4(1, 0, 0, 1));
				m_demo4Scene->addActor(ball);
			}
		}
		m_timeStep -= a_dt;
	}
}
/*****************************************************************************************
*  Demo 5 Collision Dectection
*****************************************************************************************/
void PhysicsApp::demo5(float a_dt)
{
	// 2D simulation
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			// world objects
			// sphere to sphere
			m_sphereA = new Sphere(glm::vec3(20.0f, 50.0f, 0.0f), glm::vec3(0.0f), 5.0f, 2.5f, glm::vec4(1, 0, 0, 1), true);
			m_sphereB = new Sphere(glm::vec3(20.0f, 20.0f, 0.0f), glm::vec3(0.0f), 10.0f, 5.0f, glm::vec4(0, 1, 0, 1), true);
			// box to box
			m_boxA = new Box(glm::vec3(-7.0f, 50.0f, 0.0f), glm::vec3(0.0f), 10.0f, 5.0f, glm::vec4(0, 0.2f, 1, 1), true);
			m_boxB = new Box(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f), 5.0f, 2.5f, glm::vec4(1, 0.2f, 0, 1), true);
			// box to sphere
			m_boxC = new Box(glm::vec3(-15.0f, 55.0f, 0.0f), glm::vec3(0.0f), 5.0f, 2.5f, glm::vec4(1, 0.2f, 0, 1), true);
			m_sphereC = new Sphere(glm::vec3(-21.0f, 40.0f, 0.0f), glm::vec3(0.0f), 10.0f, 5.0f, glm::vec4(0, 1, 0, 1), true);

			m_planeA = new Plane(glm::vec3(0.25f, -1.0f, 0.0f), 50.0f, true);
			m_planeB = new Plane(glm::vec3(-0.05f, -1.0f, 0.0f), 50.0f, true);

			m_physicsScene->addActor(m_sphereA);
			m_physicsScene->addActor(m_sphereB);
			m_physicsScene->addActor(m_boxA);
			m_physicsScene->addActor(m_boxB);
			m_physicsScene->addActor(m_boxC);
			m_physicsScene->addActor(m_sphereC);
			m_physicsScene->addActor(m_planeA);
			//m_physicsScene->addActor(m_planeB);

			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}

	}
	// 3D simulation
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen) {
			// world objects
			// box to box
			m_boxA = new Box(glm::vec3(-0.7f, 10.0f, -3.0f), glm::vec3(0.0f), 10.0f, 1.0f, glm::vec4(0, 0.2f, 1, 1));
			m_boxB = new Box(glm::vec3(0.0f, 5.0f, -3.0f), glm::vec3(0.0f), 5.0f, 0.5f, glm::vec4(1, 0.2f, 0, 1));
			// box to sphere
			m_boxC = new Box(glm::vec3(2.25f, 15.0f, 0.0f), glm::vec3(0.0f), 5.0f, 0.5f, glm::vec4(1, 0, 0.2f, 1));
			m_sphereA = new Sphere(glm::vec3(1.0f, 12.0f, 0.0f), glm::vec3(0.0f), 10.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			// sphere to sphere
			m_sphereB = new Sphere(glm::vec3(-5.0f, 5.0f, 0.0f), glm::vec3(0.0f), 10.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			m_sphereC = new Sphere(glm::vec3(-5.0f, 10.0f, 0.0f), glm::vec3(0.0f), 5.0f, 0.5f, glm::vec4(1, 0, 0, 1));

			m_planeA = new Plane(glm::vec3(0.0f, 1.0f, 0.0f), 20.0f);
			//m_planeB = new Plane(glm::vec3(0.0f, 1.0f, 0.0f), 10.0f);
			m_physicsScene->addActor(m_sphereA);
			m_physicsScene->addActor(m_boxA);
			m_physicsScene->addActor(m_boxB);
			m_physicsScene->addActor(m_boxC);
			m_physicsScene->addActor(m_sphereB);
			m_physicsScene->addActor(m_sphereC);
			m_physicsScene->addActor(m_planeA);

			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}
	}
}
/*****************************************************************************************
*  Demo 6 Collision Response
*****************************************************************************************/
void PhysicsApp::demo6(float a_dt)
{
	// 2D simulation
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			// world objects
			// sphere to sphere
			m_sphereA = new Sphere(glm::vec3(-20.0f, 25.0f, 0.0f), glm::vec3(0.0f), 10.0f, 5.0f, glm::vec4(0, 1, 0, 1), true);
			m_sphereA->rigidbody()->data.isKinematic = false;
			m_sphereB = new Sphere(glm::vec3(-20.0f, 10.0f, 0.0f), glm::vec3(0.0f), 5.0f, 2.5f, glm::vec4(1, 0, 0, 1), true);
			m_sphereB->rigidbody()->data.isKinematic = false;
			// box to box
			m_boxA = new Box(glm::vec3(0.0f, 6.0f, false), glm::vec3(0.0f), 10.0f, 5.0f, glm::vec4(0, 0.2f, 1, 1), true);
			m_boxA->rigidbody()->data.isKinematic = false;
			m_boxA->rigidbody()->data.onGround = true;
			m_boxB = new Box(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f), 5.0f, 2.5f, glm::vec4(1, 0.2f, 0, 1), true);
			m_boxB->rigidbody()->data.isKinematic = false;

			// box to sphere
			m_boxC = new Box(glm::vec3(20.0f, 55.0f, 0.0f), glm::vec3(0.0f), 5.0f, 2.5f, glm::vec4(1, 0.2f, 0, 1), true);
			m_boxC->rigidbody()->data.isKinematic = false;
			m_sphereC = new Sphere(glm::vec3(20.0f, 10.0f, 0.0f), glm::vec3(0.0f), 10.0f, 5.0f, glm::vec4(0, 1, 0, 1), true);
			m_sphereC->rigidbody()->data.isKinematic = false;
			m_sphereC->rigidbody()->data.onGround = true;

			// planes
			m_planeA = new Plane(glm::vec3(0.25f, -1.0f, 0.0f), 50.0f, true);
			m_planeB = new Plane(glm::vec3(-0.05f, -1.0f, 0.0f), 50.0f, true);

			m_physicsScene->addActor(m_sphereA);
			m_physicsScene->addActor(m_sphereB);
			m_physicsScene->addActor(m_boxA);
			m_physicsScene->addActor(m_boxB);
			m_physicsScene->addActor(m_boxC);
			m_physicsScene->addActor(m_sphereC);
			m_physicsScene->addActor(m_planeA);

			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}

	}
	// 3D simulation
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen) {
			// world objects
			// box to box
			m_boxA = new Box(glm::vec3(1.0f, 1.50f, -3.0f), glm::vec3(0.0f), 10.0f, 1.0f, glm::vec4(0, 0.2f, 1, 1));
			m_boxA->rigidbody()->data.isKinematic = false;
			m_boxA->rigidbody()->data.onGround = true;
			m_boxB = new Box(glm::vec3(1.0f, 10.0f, -3.0f), glm::vec3(0.0f), 5.0f, 0.5f, glm::vec4(1, 0.2f, 0, 1));
			m_boxB->rigidbody()->data.isKinematic = false;
			// box to sphere
			m_boxC = new Box(glm::vec3(1.0f, 10.0f, 0.0f), glm::vec3(0.0f), 5.0f, 0.5f, glm::vec4(1, 0, 0.2f, 1));
			m_boxC->rigidbody()->data.isKinematic = false;
			m_sphereA = new Sphere(glm::vec3(1.0f, 1.25f, 0.0f), glm::vec3(0.0f), 10.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			m_sphereA->rigidbody()->data.isKinematic = false;
			m_sphereA->rigidbody()->data.onGround = true;
			// sphere to sphere
			m_sphereB = new Sphere(glm::vec3(-5.0f, 10.0f, 0.0f), glm::vec3(0.0f), 10.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			m_sphereB->rigidbody()->data.isKinematic = false;
			m_sphereC = new Sphere(glm::vec3(-5.0f, 5.0f, 0.0f), glm::vec3(0.0f), 5.0f, 0.5f, glm::vec4(1, 0, 0, 1));
			m_sphereC->rigidbody()->data.isKinematic = false;

			m_planeA = new Plane(glm::vec3(-0.25f, 1.0f, 0.0f), 20.0f);
			//m_planeB = new Plane(glm::vec3(0.0f, 1.0f, 0.0f), 10.0f);
			m_physicsScene->addActor(m_sphereA);
			m_physicsScene->addActor(m_boxC);
			m_physicsScene->addActor(m_boxA);
			m_physicsScene->addActor(m_boxB);
			m_physicsScene->addActor(m_sphereB);
			m_physicsScene->addActor(m_sphereC);
			m_physicsScene->addActor(m_planeA);

			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}
	}
}
/*****************************************************************************************
*  Demo 7 Drag
*****************************************************************************************/
void PhysicsApp::demo7(float a_dt)
{
	// 2D simulation
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen) {
			// world objects
			m_sphereA = new Sphere(glm::vec3(-20.0f, 3.0f, 0.0f), glm::vec3(0.0f), 7.0f, 3.5f, glm::vec4(0, 0.5, 1, 1), true);
			m_sphereA->rigidbody()->data.isKinematic = false;
			m_sphereA->rigidbody()->data.rotationLock = false;
			m_sphereA->rigidbody()->data.linearDrag = 0.99f;
			m_sphereA->rigidbody()->data.angularDrag = 0.95f;
			m_sphereA->rigidbody()->data.elasticity = 0.6f;

			m_sphereB = new Sphere(glm::vec3(20.0f, 10.0f, 0.0f), glm::vec3(0.0f), 10.0f, 5.0f, glm::vec4(0, 1, 0, 1), true);
			m_sphereB->rigidbody()->data.isKinematic = false;
			m_sphereB->rigidbody()->data.rotationLock = false;
			m_sphereB->rigidbody()->data.linearDrag = 0.99f;
			m_sphereB->rigidbody()->data.angularDrag = 0.9f;
			m_sphereB->rigidbody()->data.elasticity = 0.8f;
			
			m_boxB = new Box(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f), 5.0f, 2.5f, glm::vec4(1, 0.2f, 0, 1), true);
			m_boxB->rigidbody()->data.isKinematic = false;
			m_boxB->rigidbody()->data.rotationLock = false;
			m_boxB->rigidbody()->data.linearDrag = 0.98f;
			m_boxB->rigidbody()->data.angularDrag = 0.85f;
			m_boxB->rigidbody()->data.elasticity = 0.5f;
			m_boxB->rigidbody()->data.rotation = glm::vec3(0.01f, 0.0f, 45.0f);

			// planes
			m_planeA = new Plane(glm::vec3(0.05f, -1.0f, 0.0f), 50.0f, true);

			m_physicsScene->addActor(m_sphereA);
			m_physicsScene->addActor(m_sphereB);
			m_physicsScene->addActor(m_boxB);
			m_physicsScene->addActor(m_planeA);

			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}

	}
	// 3D simulation
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen) {
			// world objects
			// box to box
			m_boxB = new Box(glm::vec3(1.0f, 10.0f, -3.0f), glm::vec3(0.0f), 5.0f, 0.5f, glm::vec4(1, 0.2f, 0, 1));
			m_boxB->rigidbody()->data.isKinematic = true;
			m_boxB->rigidbody()->data.isKinematic = false;
			m_boxB->rigidbody()->data.rotationLock = false;
			m_boxB->rigidbody()->data.linearDrag = 0.98f;
			m_boxB->rigidbody()->data.angularDrag = 0.85f;
			m_boxB->rigidbody()->data.elasticity = 0.5f;
			m_boxB->rigidbody()->data.rotation = glm::vec3(0.01f, 0.0f, 45.0f);

			m_sphereA = new Sphere(glm::vec3(5.0f, 1.0f, 3.0f), glm::vec3(0.0f), 7.0f, 0.7f, glm::vec4(0, 0.5, 1, 1));
			m_sphereA->rigidbody()->data.isKinematic = false;
			m_sphereA->rigidbody()->data.rotationLock = false;
			m_sphereA->rigidbody()->data.linearDrag = 0.99f;
			m_sphereA->rigidbody()->data.angularDrag = 0.9f;
			m_sphereA->rigidbody()->data.elasticity = 0.8f;

			m_sphereB = new Sphere(glm::vec3(-5.0f, 10.0f, 0.0f), glm::vec3(0.0f), 10.0f, 1.0f, glm::vec4(0, 1, 0, 1));
			m_sphereB->rigidbody()->data.isKinematic = false;
			m_sphereB->rigidbody()->data.rotationLock = false;
			m_sphereB->rigidbody()->data.linearDrag = 0.99f;
			m_sphereB->rigidbody()->data.angularDrag = 0.85f;
			m_sphereB->rigidbody()->data.elasticity = 0.8f;

			m_planeA = new Plane(glm::vec3(-0.05f, 1.0f, 0.0f), 20.0f);

			m_physicsScene->addActor(m_boxB);
			m_physicsScene->addActor(m_sphereA);
			m_physicsScene->addActor(m_sphereB);
			m_physicsScene->addActor(m_planeA);

			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}
	}
}
/*****************************************************************************************
*  Demo 8 Springs
*****************************************************************************************/
void PhysicsApp::demo8(float a_dt)
{
	// 2D simulation
	if (m_render2D && !m_render3D) {
		if (!m_renderChosen && m_optionsD8) {
			// ball positions
			int startX = 0;
			float radius = 2;
			float mass = 5;
			float posY = 50.0f;
			glm::vec4 colour = glm::vec4(0, 0, 1, 1);
			
			// world objects
			m_sphereA = new Sphere(glm::vec3(startX, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour, true);
			m_sphereA->rigidbody()->data.isKinematic = false;
			m_sphereA->rigidbody()->data.rotationLock = false;
			m_sphereA->rigidbody()->data.linearDrag = 0.01f;
			m_sphereA->rigidbody()->data.angularDrag = 0.9;
			m_sphereA->rigidbody()->data.elasticity = 0.1f;

			// planes
			m_planeA = new Plane(glm::vec3(0.05f, -1.0f, 0.0f), 50.0f, true);

			// add first ball
			m_physicsScene->addActor(m_sphereA);

			int ballCount = 10;
			for (int i = 1; i < ballCount; i++) {
				// update colour
				colour = glm::vec4(colour.x, colour.y + 0.1f , colour.z - 0.1f, 1);
				// next sphere
				m_sphereB = new Sphere(glm::vec3(startX + i*m_restLength, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour, true);
				m_sphereB->rigidbody()->data.isKinematic = false;
				m_sphereB->rigidbody()->data.rotationLock = false;
				m_sphereB->rigidbody()->data.linearDrag = 0.99f;
				m_sphereB->rigidbody()->data.angularDrag = 0.9f;
				m_sphereB->rigidbody()->data.elasticity = 0.1f;
				// add to scene
				m_physicsScene->addActor(m_sphereB);
				m_physicsScene->addActor(new SpringJoint(m_sphereA->rigidbody(), m_sphereB->rigidbody(), m_spring, m_damping, true));
				m_sphereA = m_sphereB;
			}

			//m_physicsScene->addActor(m_planeA);
			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}

	}
	// 3D simulation
	if (m_render3D && !m_render2D) {
		if (!m_renderChosen && m_optionsD8) {
			// ball positions
			float startX = 0.0f;
			float posY = 15.0f;
			float radius = 1.0f;
			float mass = 5.0f;
			glm::vec4 colour = glm::vec4(0, 0, 1, 1);
			m_cameraView = glm::vec3(1.01f, 20.0f, 44.0f);
			// world objects
			m_sphereA = new Sphere(glm::vec3(startX, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour);
			m_sphereA->rigidbody()->data.isKinematic = false;
			m_sphereA->rigidbody()->data.rotationLock = false;
			m_sphereA->rigidbody()->data.linearDrag = 0.01f;
			m_sphereA->rigidbody()->data.angularDrag = 0.9f;
			m_sphereA->rigidbody()->data.elasticity = 0.1f;
			// add to scene
			m_physicsScene->addActor(m_sphereA);

			int ballCount = 10;
			for (int i = 1; i < ballCount; i++) {
				// update colour
				colour = glm::vec4(colour.x, colour.y + 0.1f, colour.z - 0.1f, 1);
				// next sphere
				m_sphereB = new Sphere(glm::vec3(startX + (float)i*m_restLength, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour);
				m_sphereB->rigidbody()->data.isKinematic = false;
				m_sphereB->rigidbody()->data.rotationLock = false;
				m_sphereB->rigidbody()->data.linearDrag = 0.99f;
				m_sphereB->rigidbody()->data.angularDrag = 0.9f;
				m_sphereB->rigidbody()->data.elasticity = 0.1f;
				// add to scene
				m_physicsScene->addActor(m_sphereB);
				m_physicsScene->addActor(new SpringJoint(m_sphereA->rigidbody(), m_sphereB->rigidbody(), m_spring, m_damping));
				m_sphereA = m_sphereB;
			}
			// plane
			m_planeA = new Plane(glm::vec3(-0.05f, 1.0f, 0.0f), 20.0f);
			// add to scene
			//m_physicsScene->addActor(m_planeA);
			// finalise scene
			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}
	}
	// if options have been selected continue with simulation
	if (m_optionsD8) {
		fixedUpdate(a_dt);
	}
}
/*****************************************************************************************
*  Simulation
*****************************************************************************************/
void PhysicsApp::simulation(float a_dt, aie::Input* input)
{
	// 3D simulation
	if (!m_render3D && m_render2D) {
		if (!m_renderChosen) {
			// ------------------------- shot ball ------------------------------
			Sphere * m_shotBall = new Sphere(glm::vec3(0.0f, -40.0f, 0.0f), glm::vec3(0.0f), 10.0f, 2.0f, glm::vec4(0, 1, 0, 1), true);
			m_shotBall->rigidbody()->data.isStatic = true;
			m_shotBall->rigidbody()->data.isKinematic = true;
			m_physicsScene->addActor(m_shotBall);

			// -------------------------- targets -------------------------------
			glm::vec4 colour = glm::vec4(0, 0, 1, 1);
			for (int i = 0; i < 11; i++) {
				// update colour
				colour = glm::vec4(colour.x + 0.1f, colour.y, colour.z - 0.1f, 1);
				Box * boxA = new Box(glm::vec3(-50.0f + i * 10, 40.0f, 0.0f), glm::vec3(0.0f), 2.0f, 2.0f, colour, true);
				//boxA->rigidbody()->data.isStatic = true;
				boxA->rigidbody()->data.isKinematic = false;
				boxA->rigidbody()->data.onGround = true;
				boxA->rigidbody()->data.rotationLock = false;
				boxA->rigidbody()->data.linearDrag = 0.99f;
				boxA->rigidbody()->data.angularDrag = 0.9f;
				boxA->rigidbody()->data.elasticity = 0.5f;
				m_physicsScene->addActor(boxA);
			}

			// -------------------------- spring dangles ------------------------
			sphereDangle(-80, 30);
			squareDangle(80, 30);

			// -------------------------- static seive -------------------------------
			for (int i = 0; i < 11; i++) {
				// update colour
				colour = glm::vec4(colour.x - 0.1f, colour.y, colour.z + 0.1f, 1);
				Sphere * boxA = new Sphere(glm::vec3(-50.0f + i * 10, -50.0f, 0.0f), glm::vec3(0.0f), 2.0f, 2.0f, colour, true);
				boxA->rigidbody()->data.isStatic = true;
				//boxA->rigidbody()->data.isKinematic = true;
				boxA->rigidbody()->data.onGround = true;
				m_physicsScene->addActor(boxA);
			}

			// finalise scene
			m_physicsScene->setGravity(glm::vec3(0.0f, -10.f, 0.0f));
			m_physicsScene->properties.gravity = true;
			m_physicsScene->properties.collisions = true;
			m_renderChosen = true;
		}
		else if(m_renderChosen && m_sceneReady)  {
			// detect mouse clicks
			if (input->isMouseButtonDown(0)) {
				if (!m_mouseClicked) {
					// save initial click position
					input->getMouseXY(&m_firstClickPos.x, &m_firstClickPos.y);
					m_mouseClicked = true;
					m_shoot = false;
					m_physicsScene->actors()[0]->rigidbody()->data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
					m_physicsScene->actors()[0]->rigidbody()->data.position = glm::vec3(0.0f, -40.0f, 0.0f);
					m_physicsScene->actors()[0]->rigidbody()->data.isStatic = true;
					m_physicsScene->actors()[0]->rigidbody()->data.isKinematic = true;
						std::cout << "mouse clicked: " << m_firstClickPos.x << " ," << m_firstClickPos.y << std::endl;
				}
				else {
					// save mouse current screeen position
					input->getMouseXY(&m_mouseScreenPos.x, &m_mouseScreenPos.y);
				}
			}
			// mouse input
			if (input->isMouseButtonUp(0) && m_mouseClicked) {
				m_mouseClicked = false;
				m_shoot = true;
				m_unClickPos = m_mouseScreenPos;
				m_springForce = glm::distance(glm::vec2(m_firstClickPos * 2), glm::vec2(m_unClickPos * 2));
				std::cout << "spring force: " << m_springForce << std::endl;
				std::cout << "mouse released: " << m_unClickPos.x << " ," << m_unClickPos.y << std::endl;
			}

			if (m_shoot && m_springForce > 0) {
				glm::vec2 direction = m_firstClickPos - m_unClickPos;
				glm::vec3 force = glm::vec3(direction.x, direction.y, 0.0f);
				m_physicsScene->actors()[0]->rigidbody()->data.isStatic = false;
				m_physicsScene->actors()[0]->rigidbody()->data.isKinematic = false;
				m_physicsScene->actors()[0]->rigidbody()->applyForce(force);
				m_shoot = false;
			}
		}
	}

}

void PhysicsApp::squareDangle(float xPos, float yPos)
{
	// box positions
	int startX = xPos;
	float radius = 2;
	float mass = 5;
	float posY = yPos;
	glm::vec4 colour = glm::vec4(0, 0, 1, 1);
	// spring settings
	m_spring = 0.5f;
	m_damping = 0.5f;
	m_restLength = 7.0f;

	// world objects
	m_boxA = new Box(glm::vec3(startX, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour, true);
	m_boxA->rigidbody()->data.isKinematic = false;
	m_boxA->rigidbody()->data.rotationLock = false;
	m_boxA->rigidbody()->data.linearDrag = 0.01f;
	m_boxA->rigidbody()->data.angularDrag = 0.9;
	m_boxA->rigidbody()->data.elasticity = 0.1f;

	// planes
	m_planeA = new Plane(glm::vec3(0.05f, -1.0f, 0.0f), 50.0f, true);

	// add first box
	m_physicsScene->addActor(m_boxA);

	int boxCount = 10;
	for (int i = 1; i < boxCount; i++) {
		// update colour
		colour = glm::vec4(colour.x, colour.y + 0.1f, colour.z - 0.1f, 1);
		// next box
		m_boxB = new Box(glm::vec3(startX - i*m_restLength, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour, true);
		m_boxB->rigidbody()->data.isKinematic = false;
		m_boxB->rigidbody()->data.rotationLock = false;
		m_boxB->rigidbody()->data.linearDrag = 0.99f;
		m_boxB->rigidbody()->data.angularDrag = 0.9f;
		m_boxB->rigidbody()->data.elasticity = 0.1f;
		// add to scene
		m_physicsScene->addActor(m_boxB);
		m_physicsScene->addActor(new SpringJoint(m_boxA->rigidbody(), m_boxB->rigidbody(), m_spring, m_damping, true));
		m_boxA = m_boxB;
	}
}

void PhysicsApp::sphereDangle(float xPos, float yPos)
{
	// ball positions
	int startX = xPos;
	float radius = 2;
	float mass = 5;
	float posY = yPos;
	glm::vec4 colour = glm::vec4(0, 0, 1, 1);
	// spring settings
	m_spring = 0.5f;
	m_damping = 0.5f;
	m_restLength = 7.0f;

	// world objects
	m_sphereA = new Sphere(glm::vec3(startX, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour, true);
	m_sphereA->rigidbody()->data.isKinematic = false;
	m_sphereA->rigidbody()->data.rotationLock = false;
	m_sphereA->rigidbody()->data.linearDrag = 0.01f;
	m_sphereA->rigidbody()->data.angularDrag = 0.9;
	m_sphereA->rigidbody()->data.elasticity = 0.1f;

	// planes
	m_planeA = new Plane(glm::vec3(0.05f, -1.0f, 0.0f), 50.0f, true);

	// add first ball
	m_physicsScene->addActor(m_sphereA);

	int ballCount = 10;
	for (int i = 1; i < ballCount; i++) {
		// update colour
		colour = glm::vec4(colour.x, colour.y + 0.1f, colour.z - 0.1f, 1);
		// next sphere
		m_sphereB = new Sphere(glm::vec3(startX + i*m_restLength, posY, 0.0f), glm::vec3(0.0f), mass, radius, colour, true);
		m_sphereB->rigidbody()->data.isKinematic = false;
		m_sphereB->rigidbody()->data.rotationLock = false;
		m_sphereB->rigidbody()->data.linearDrag = 0.99f;
		m_sphereB->rigidbody()->data.angularDrag = 0.9f;
		m_sphereB->rigidbody()->data.elasticity = 0.1f;
		// add to scene
		m_physicsScene->addActor(m_sphereB);
		m_physicsScene->addActor(new SpringJoint(m_sphereA->rigidbody(), m_sphereB->rigidbody(), m_spring, m_damping, true));
		m_sphereA = m_sphereB;
	}

	// end sphere
	//m_sphereB = new Box(glm::vec3(endX, posY, 0.0f), glm::vec3(0.0f), mass, radius, glm::vec4(0, 0.2f, 1, 1));
	//m_sphereB->rigidbody()->data.isKinematic = false;
	//m_sphereB->rigidbody()->data.rotationLock = true;
	//m_sphereB->rigidbody()->data.linearDrag = 0.01f;
	//m_sphereB->rigidbody()->data.angularDrag = 0.9f;
	//m_sphereB->rigidbody()->data.elasticity = 0.1f;
	//// add to scene
	//m_physicsScene->addActor(m_sphereB);
	//m_physicsScene->addActor(new SpringJoint(m_sphereA->rigidbody(), m_sphereB->rigidbody(), m_spring, m_damping));
}


