#include "Plane.h"
#include "Gizmos.h"
#include <iostream>


Plane::Plane()
{
	m_shapeID = PLANE;
	m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
	m_distanceToOrigin = 200.0f;
	m_2D = false;
}

Plane::Plane(glm::vec3 normal, float distance, bool twoD)
{
	m_shapeID = PLANE;
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_2D = twoD;
}


Plane::~Plane()
{
}

void Plane::fixedUpdate(glm::vec3 gravity, float timeStep)
{
}

void Plane::debug()
{
}

void Plane::makeGizmo()
{
	if (m_2D) {
		float lineLength = 300;
		glm::vec2 centerPoint(m_normal.x * m_distanceToOrigin, m_normal.y * m_distanceToOrigin);
		glm::vec2 parallel(m_normal.y, -m_normal.x);
		glm::vec4 colour(0.0f, 0.0f, 0.5f, 1.0f);
		glm::vec2 start = centerPoint + (parallel * lineLength);
		glm::vec2 end = centerPoint - (parallel * lineLength);
		aie::Gizmos::add2DLine(start, end, colour);
	}
	else {
		// draw a simple grid with gizmos
		glm::vec4 blue(0.0f, 0.0f, 0.5f, 1.0f);
		for (int i = 0; i < 21; ++i) {
			aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
								glm::vec3(-10 + i, 0, -10),
								blue);
			aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i),
								glm::vec3(-10, 0, -10 + i),
								blue);
		}
	}
}