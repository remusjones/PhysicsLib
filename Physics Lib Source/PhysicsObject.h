#pragma once
#include <glm\glm.hpp>

class PhysicsShape;
enum ForceMode
{
	IMPULSE,
	ACCELERATION
};
class PhysicsObject
{
public:

	PhysicsObject();
	PhysicsObject(float mass = 1.0f);
	~PhysicsObject();

	// getters
	float GetMass() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetVelocity() const;

	// setters
	void SetMass(const float mass);
	void SetPosition(const glm::vec3 position);
	void SetVelocity(const glm::vec3 velocity);
	void AddForce(glm::vec3 force, ForceMode forceType);
	void AddAcceleration(const glm::vec3 acceleration);
	void AddVelocity(const glm::vec3 velocity);
	
	void AddShape(PhysicsShape* shape);
	void RemoveShape();
	PhysicsShape* GetShape() const;

	void Update(float deltaTime);
	void SetIsStatic(bool b);
	bool GetIsStatic();
private:
	glm::vec3		m_position;
	glm::vec3		m_velocity;
	glm::vec3		m_force;
	glm::vec3		m_acceleration;
	float			m_mass;
	PhysicsShape*	m_shape;
	bool			m_isStatic;
};

