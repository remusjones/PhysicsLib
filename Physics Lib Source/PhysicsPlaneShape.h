#pragma once
#include "PhysicsShape.h"
#include <glm\glm.hpp>
class PhysicsPlaneShape :
	public PhysicsShape
{
public:
	PhysicsPlaneShape() = delete;
	PhysicsPlaneShape(float d, glm::vec3 normal)
		: PhysicsShape(PhysicsShape::ShapeType::Plane)
		, m_dOrigin(d) 
	{
		m_normal = glm::normalize(normal);
	}
	~PhysicsPlaneShape();

	void SetNormal(glm::vec3 normal) { m_normal = glm::normalize(normal); }
	glm::vec3 GetNormal() const { return m_normal; }

	void SetDistanceOrigin(float d) { m_dOrigin = d; }
	float GetDistanceOrigin() const { return m_dOrigin; }

private:
	float m_dOrigin;
	glm::vec3 m_normal;
};

