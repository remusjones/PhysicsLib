#include "PhysicsObject.h"
#include "PhysicsShape.h"


PhysicsObject::PhysicsObject()
	: PhysicsObject(1.0f)
{

}

PhysicsObject::PhysicsObject(float mass)
	: m_mass(mass)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_force(0.0f,0.0f,0.0f)
	, m_acceleration(0.0f,0.0f,0.0f)
	, m_shape(nullptr)
	, m_isStatic(false)
{

}


PhysicsObject::~PhysicsObject()
{
	RemoveShape();
}

void PhysicsObject::SetMass(const float mass)
{
	m_mass = mass;
}

float PhysicsObject::GetMass() const
{
	return m_mass;
}

void PhysicsObject::SetVelocity(const glm::vec3 velocity)
{
	m_velocity = velocity;
}

void PhysicsObject::AddForce(glm::vec3 force, ForceMode forceType)
{
	switch (forceType)
	{
	case IMPULSE:
		m_force += force;
		break;
	case ACCELERATION:
		AddAcceleration(force);
		break;
	}
	
}

void PhysicsObject::AddAcceleration(const glm::vec3 acceleration)
{
	m_acceleration += acceleration;
}

void PhysicsObject::AddVelocity(const glm::vec3 velocity)
{
	m_velocity += velocity;
}

void PhysicsObject::AddShape(PhysicsShape * shape)
{
	m_shape = shape;
}

void PhysicsObject::RemoveShape()
{
	if (m_shape != nullptr)
	{
		delete m_shape;
		m_shape = nullptr;
	}
}

PhysicsShape * PhysicsObject::GetShape() const
{
	return m_shape;
}

void PhysicsObject::Update(float deltaTime)
{
	// start of frame
	// calculate acceleration
	m_acceleration += m_force / m_mass;
	// calculate velocity
	m_velocity += (m_acceleration * deltaTime);
	// apply velocity
	m_position += (m_velocity * deltaTime);





	// end of frame
	// clear force
	m_force = glm::vec3(0.0f, 0.0f, 0.0f);
	m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}
void PhysicsObject::SetIsStatic(bool b)
{
	m_isStatic = b;
}
bool PhysicsObject::GetIsStatic()
{
	return m_isStatic;
}
;

glm::vec3 PhysicsObject::GetVelocity() const
{
	return m_velocity;
}

void PhysicsObject::SetPosition(const glm::vec3 position)
{
	m_position = position;
}

glm::vec3 PhysicsObject::GetPosition() const
{
	return m_position;
}
