#pragma once
class PhysicsShape
{
public:
	enum class ShapeType 
	{
		Plane, 
		AABB,
		Sphere,
		NUM_SHAPES
	};

	PhysicsShape() = delete;
	PhysicsShape(ShapeType type) : m_type(type){};

	~PhysicsShape() {}
	ShapeType GetType() const { return m_type; }
private:
	ShapeType m_type;
};

