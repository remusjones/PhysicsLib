#include "GameObject.h"
#include "PhysicsObject.h"
#include <Gizmos.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "PhysicsSphereShape.h"
#include "PhysicsAABBShape.h"
#include "PhysicsPlaneShape.h"

GameObject::GameObject()
	: m_physicsObject(nullptr)
{
}

GameObject::~GameObject()
{
}

void GameObject::DebugPhysicsRender()
{
	if (m_physicsObject != nullptr)
	{
		glm::vec3 position = m_physicsObject->GetPosition();
		//aie::Gizmos::addSphere(position, 1.0f, 15, 15, glm::vec4(1, 0, 0, 1));

		PhysicsShape* pShape = m_physicsObject->GetShape();
		if (pShape == nullptr)
			return;

		switch (pShape->GetType())
		{
		case PhysicsShape::ShapeType::AABB:
		{
			PhysicsAABBShape* pAABB = (PhysicsAABBShape*)pShape;
			aie::Gizmos::addAABB(position, pAABB->GetExtents(), glm::vec4(1, 0, 0, 1));

			break;
		}
		case PhysicsShape::ShapeType::Sphere:
		{
			PhysicsSphereShape* pSphere = (PhysicsSphereShape*)pShape;
			aie::Gizmos::addSphere(position, pSphere->GetRadius(), 15, 15, glm::vec4(1, 0, 0, 1));
			break;
		}
		case PhysicsShape::ShapeType::Plane:
		{
			PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)pShape;
			glm::vec3 right(pPlane->GetNormal());
			glm::vec3 forward;
			glm::vec3 up;

			glm::vec3 worldup(0, 1, 0);
			glm::vec3 worldForward(0, 0, 1);

			if (glm::dot(right, worldup) < 0.99f)
			{
				forward = glm::cross(right, worldup);
				up = glm::cross(forward, right);
			}
			else
			{
				up = glm::vec3(-1, 0, 0);
				forward = worldForward;
			}

			glm::mat4 rotation(1);
			rotation[0] = glm::vec4(right, 0);
			rotation[1] = glm::vec4(up, 0);
			rotation[2] = glm::vec4(forward, 0);
			rotation[3] = glm::vec4(pPlane->GetDistanceOrigin() * pPlane->GetNormal(), 1);

			glm::vec3 offset;
			glm::vec3 normal = pPlane->GetNormal();
			//float dist = pPlane->GetDistanceOrigin();
			//if (normal.x != 0.0f)
			//{
			//	if (normal.x > 0.0f)
			//		offset.x += dist;
			//	else
			//		offset.x -= dist;
			//}
			//if (normal.y != 0.0f)
			//{
			//	if (normal.y > 0.0f)
			//		offset.y += dist;
 			//	else
			//		offset.y -= dist;
			//}
			//if (normal.z != 0.0f)
			//{
			//	if (normal.z > 0.0f)
			//		offset.z += dist;
			//	else
			//		offset.z -= dist;
			//}
			
			aie::Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(0.3f, 100, 100), glm::vec4(0, 0, 1, 1), &rotation);
			break;

			break;
		}
		default:
			break;
		}

	}
}

void GameObject::SetPhysicsObject(PhysicsObject * physicsObject)
{
	m_physicsObject = physicsObject;
}

PhysicsObject * GameObject::GetPhysicsObject() const
{
	return m_physicsObject;
}
