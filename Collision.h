#pragma once
#include "PhysicsObject.h"


#include "PhysicsAABBShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsPlaneShape.h"

#include <glm\glm.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>
class PhysicsShape;
namespace Collision{

	// aabb vs other
	static bool CheckAABBCollision(PhysicsObject& a, PhysicsObject& b)
	{
		PhysicsAABBShape* aAABB = (PhysicsAABBShape*)a.GetShape();
		PhysicsAABBShape* bAABB = (PhysicsAABBShape*)b.GetShape();
	
		glm::vec3 aMin = a.GetPosition() - aAABB->GetExtents();
		glm::vec3 bMin = b.GetPosition() - bAABB->GetExtents();
	
		glm::vec3 aMax = a.GetPosition() + aAABB->GetExtents();
		glm::vec3 bMax = b.GetPosition() + bAABB->GetExtents();
	
	
		return  (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
	}
//	static CheckAABBCollision(PhysicsObject & a, PhysicsObject & b);
	static bool CheckAABBvsSphere(PhysicsObject& a, PhysicsObject& s)
	{
		PhysicsAABBShape* aAABB = (PhysicsAABBShape*)a.GetShape();
		PhysicsSphereShape* bSphere = (PhysicsSphereShape*)s.GetShape();
	
		// min max extents
		glm::vec3 aMin = a.GetPosition() - aAABB->GetExtents();
		glm::vec3 aMax = a.GetPosition() + aAABB->GetExtents();
	
		glm::vec3 bSpherePos = s.GetPosition();
		// clamp the points
		float x = std::max(aMin.x, std::min(bSpherePos.x, aMax.x));
		float y = std::max(aMin.y, std::min(bSpherePos.y, aMax.y));
		float z = std::max(aMin.z, std::min(bSpherePos.z, aMax.z));
	
		float distance = std::sqrt((x - bSpherePos.x) * (x - bSpherePos.x) +
			(y - bSpherePos.y) * (y - bSpherePos.y) +
			(z - bSpherePos.z) * (z - bSpherePos.z));
	
		return distance < bSphere->GetRadius();
	
		return false;
	}
	static bool CheckAABBvsPlane(PhysicsObject& a, PhysicsObject& plane)
	{
	
	}
	static bool CheckAABBvsPoint(PhysicsObject& a, PhysicsObject& p);
	
	// sphere vs other
	static bool CheckSpherevsPlane(PhysicsObject& a, PhysicsObject& plane)
	{
		PhysicsSphereShape* aSphere = (PhysicsSphereShape*)a.GetShape();
		PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)plane.GetShape();
	
		if (aSphere == nullptr || pPlane == nullptr)
			return false;
	
		if (a.GetShape()->GetType() != PhysicsShape::ShapeType::Sphere)
		{
			std::cout << "Collision Mismatch" << std::endl;
			return false;
		}
		if (plane.GetShape()->GetType() != PhysicsShape::ShapeType::Plane)
		{
			std::cout << "Collision Mismatch" << std::endl;
			return false;
		}
	
	
		glm::vec3 collisionNormal = pPlane->GetNormal();
		float sphereToPlane = glm::dot
								(a.GetPosition(),
								pPlane->GetNormal()) -
								pPlane->GetDistanceOrigin();
	
	
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}
	
	
		float intersection = aSphere->GetRadius() - sphereToPlane;
		if (intersection > 0)
		{
			//a.SetVelocity(glm::vec3(0, 0, 0));
			glm::vec3 planeNormal = pPlane->GetNormal();
	
			if (sphereToPlane < 0)
				planeNormal *= -1;
	
			glm::vec3 forceVector = -1 * a.GetMass() * planeNormal *
				(glm::dot(planeNormal, a.GetVelocity()));
	
			a.AddVelocity(2.0f * forceVector);
			a.SetPosition(a.GetPosition() + collisionNormal *
				intersection * 0.5f);
	
			return true;
		}
	
	
		return false;
	}
	static bool CheckSpherevsPoint(PhysicsObject& a, PhysicsObject& p);
	static bool CheckSpherevsSphere(PhysicsObject& a, PhysicsObject& b)
	{
		// get the positions and store them
		glm::vec3 aPos = a.GetPosition();
		glm::vec3 bPos = b.GetPosition();
		// cast the objects using polymorphism
		PhysicsSphereShape* aSphere = (PhysicsSphereShape*)a.GetShape();
		PhysicsSphereShape* bSphere = (PhysicsSphereShape*)b.GetShape();
	
	
	
		glm::vec3 delta = aPos - bPos;
		float distance = glm::length(delta);
		float intersection = aSphere->GetRadius() + bSphere->GetRadius() - distance;
	
		if (intersection > 0)
		{
			glm::vec3 collisionNormal = glm::normalize(delta);
			glm::vec3 relativeVelocity = a.GetVelocity() - b.GetVelocity();
			glm::vec3 collisionVector = collisionNormal* (glm::dot(relativeVelocity, collisionNormal));
			glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / a.GetMass() + 1.0f / b.GetMass());
	
			a.SetVelocity(-forceVector * 2.0f);
			b.SetVelocity(forceVector * 2.0f);
	
			glm::vec3 seperationVector = collisionNormal*intersection * 0.5f;
			a.SetPosition(a.GetPosition() - seperationVector);
			b.SetPosition(b.GetPosition() - seperationVector);
			return true;
	
	
		}
		return false;
	//	// we are using multiplications because it's faster than calling Math.pow
	//	// get the distance
	//	float distance = std::sqrt(
	//		(aPos.x - bPos.x) * (aPos.x - bPos.x) +
	//		(aPos.y - bPos.y) * (aPos.y - bPos.y) +
	//		(aPos.z - bPos.z) * (aPos.z - bPos.z));
	//	// check if distance is within the radius
	//	return distance < (aSphere->GetRadius() + bSphere->GetRadius());
	}
	
	// plane vs other
	static bool CheckPlanevsPlane(PhysicsObject& a, PhysicsObject& b);
	//struct CollisionInfo
	//{
	//	bool		wasCollision;
	//	glm::vec3	normal;
	//	float		interceptDistance;
	//};
	//
	//
	//bool CheckCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
	//{
	//	// create a function pointer type for our collision functions
	//	typedef bool(*collisionFnc)(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	//
	//	// create an array with all of our collision functions
	//	// these are ordered based on the "shapeType" enum order
	//	static collisionFnc collisionFunctionArray[] =
	//	{
	//		nullptr,nullptr,CheckPlaneSphereCollision
	//		, nullptr, nullptr, nullptr,
	//		CheckPlaneSphereCollision, nullptr, nullptr
	//
	//	};
	//	//'hash' the two shapeIds to create a unique index
	//	auto index = ((int)obj1->GetShape()->GetType() *
	//		(int)PhysicsShape::ShapeType::NUM_SHAPES) +
	//		(int)(obj2->GetShape()->GetType());
	//	// uf we have a collision function for this combination of shapes, call the collision function.
	//	if (collisionFunctionArray[index] != nullptr)
	//	{
	//		return collisionFunctionArray[index](obj1, obj2, collisionInfo);
	//	}
	//
	//
	//	// otherwise, return false
	//	return false;
	//}
	//// Specific Shape Collision Funcs
	//bool CheckPlaneSphereCollision(const PhysicsObject* pPlane, const PhysicsObject* pSphere, CollisionInfo& collisionInfo)
	//{
	//	bool wasCollision = CheckSpherePlaneCollision(pSphere, pPlane, collisionInfo);
	//	if (wasCollision)
	//	{
	//
	//		collisionInfo.normal = -collisionInfo.normal;
	//	}
	//	return wasCollision;
	//}
	//bool CheckSpherePlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
	//{
	//	PhysicsSphereShape* Sphere = (PhysicsSphereShape*)obj1->GetShape();
	//	PhysicsPlaneShape* Plane = (PhysicsPlaneShape*)obj2->GetShape();
	//
	//
	//	float distanceFormPlane(glm::dot(obj1->GetPosition(), Plane->GetNormal()));
	//	distanceFormPlane -= Plane->GetDistanceOrigin();
	//
	//	if (distanceFormPlane < Sphere->GetRadius())
	//	{
	//		collisionInfo.normal = -Plane->GetNormal();
	//		collisionInfo.interceptDistance = Sphere->GetRadius() - Plane->GetDistanceOrigin();
	//		collisionInfo.wasCollision = true;
	//
	//	}
	//	else
	//	{
	//		collisionInfo.wasCollision = false;
	//	}
	//
	//	return collisionInfo.wasCollision;
	//}
	//bool CheckSphereSphereCollision(const PhysicsObject* pShere1, const PhysicsObject* pSphere2, CollisionInfo& collisionInfo)
	//{
	//
	//
	//	return false;
	//}
	//
	//void ResolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo)
	//{
	//	if (collisionInfo.wasCollision)
	//	{
	//		HandleSeperation(obj1, obj2, collisionInfo);
	//		HandleVelocityChange(obj1, obj2, collisionInfo);
	//	}
	//}
	//void HandleSeperation(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo)
	//{
	//	float totalSystemMass = obj1->GetMass() + obj2->GetMass();
	//	float obj1Offset = collisionInfo.interceptDistance * (obj2->GetMass() / totalSystemMass);
	//	float obj2Offset = collisionInfo.interceptDistance * (obj1->GetMass() / totalSystemMass);
	//
	//	glm::vec3 obj1NewPosition = obj1->GetPosition() - (obj1Offset * collisionInfo.normal);
	//	glm::vec3 obj2NewPosition = obj2->GetPosition() - (obj2Offset * collisionInfo.normal);
	//
	//	obj1->SetPosition(obj1NewPosition);
	//	obj2->SetPosition(obj2NewPosition);
	//}
	//void HandleVelocityChange(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo)
	//{
	//
	//	obj1->SetVelocity(glm::vec3(0, 0, 0));
	//	obj2->SetVelocity(glm::vec3(0, 0, 0));
	//}





};

