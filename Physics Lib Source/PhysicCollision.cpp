#include "PhysicCollision.h"

bool PhysicCollision::CheckCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	// create a function pointer type for our collision functions
	typedef bool(*collisionFnc)(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// create an array with all of our collision functions
	// these are ordered based on the "shapeType" enum order
	static collisionFnc collisionFunctionArray[] =
	{
			// p2p						p2AABB			// p2s
			PlaneToPlane,				CheckPlaneAABB,	CheckPlaneSphere
			// AA2p						AABB2AABB		AABB2s
		,	CheckAABBPlane,				CheckAABBAABB,	CheckAABBSphere,
			// s2p						//s2aab			//s2s
			CheckSpherePlaneCollision,	CheckSphereABBB,CheckSphereSphereCollision

	};
	//'hash' the two shapeIds to create a unique index
	auto index = ((int)obj1->GetShape()->GetType() *
		(int)PhysicsShape::ShapeType::NUM_SHAPES) +
		(int)(obj2->GetShape()->GetType());
	// uf we have a collision function for this combination of shapes, call the collision function.
	if (collisionFunctionArray[index] != nullptr)
	{
		return collisionFunctionArray[index](obj1, obj2, collisionInfo);
	}


	// otherwise, return false
	return false;
}

bool PhysicCollision::CheckPlaneSphere(const PhysicsObject * pPlane, const PhysicsObject * pSphere, CollisionInfo & collisionInfo)
{
	bool wasCollision = CheckSpherePlaneCollision(pSphere, pPlane, collisionInfo);
	if (wasCollision)
	{

		collisionInfo.normal = -collisionInfo.normal;
	}
	return wasCollision;
}

bool PhysicCollision::CheckSpherePlaneCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	PhysicsSphereShape* Sphere = (PhysicsSphereShape*)obj1->GetShape();
	PhysicsPlaneShape* Plane = (PhysicsPlaneShape*)obj2->GetShape();


	float distanceFromPlane(glm::dot(obj1->GetPosition(), Plane->GetNormal()));
	distanceFromPlane -= Plane->GetDistanceOrigin();

	if (distanceFromPlane < Sphere->GetRadius())
	{
		collisionInfo.normal = -Plane->GetNormal();
		collisionInfo.interceptDistance = Sphere->GetRadius() - distanceFromPlane;
		collisionInfo.wasCollision = true;

	}
	else
	{
		collisionInfo.wasCollision = false;
	}

	return collisionInfo.wasCollision;
}

bool PhysicCollision::CheckSphereSphereCollision(const PhysicsObject * pSphere1, const PhysicsObject * pSphere2, CollisionInfo & collisionInfo)
{
	glm::vec3 distanceBetweenSpheres;
	distanceBetweenSpheres = pSphere1->GetPosition() - pSphere2->GetPosition();

	PhysicsSphereShape *sphere1 = (PhysicsSphereShape*)(pSphere1->GetShape());
	PhysicsSphereShape *sphere2 = (PhysicsSphereShape*)(pSphere2->GetShape());

	float distance = glm::length(distanceBetweenSpheres);
	distance -= sphere1->GetRadius() + sphere2->GetRadius();

	if (distance <= 0.0f)
	{
		collisionInfo.normal = -glm::normalize(distanceBetweenSpheres);
		collisionInfo.interceptDistance = -distance;
		collisionInfo.wasCollision = true;
	}
	else
	{
		//no collision
		collisionInfo.wasCollision = false;
	}

	return collisionInfo.wasCollision;
}

bool PhysicCollision::CheckAABBAABB(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{


	PhysicsAABBShape* aAABB = (PhysicsAABBShape*)obj1->GetShape();
	PhysicsAABBShape* bAABB = (PhysicsAABBShape*)obj2->GetShape();

	// calculate the extents
	glm::vec3 aMinExtents((glm::vec3(obj1->GetPosition()) - aAABB->GetExtents()));
	glm::vec3 aMaxExtents((glm::vec3(obj1->GetPosition()) + aAABB->GetExtents()));
	
	
	glm::vec3 bMinExtents((glm::vec3(obj2->GetPosition()) - bAABB->GetExtents()));
	glm::vec3 bMaxExtents((glm::vec3(obj2->GetPosition()) + bAABB->GetExtents()));
	
	// check if there was a collision
	collisionInfo.wasCollision =	
					(aMinExtents.x <= bMaxExtents.x && aMaxExtents.x >= bMinExtents.x) &&
					(aMinExtents.y <= bMaxExtents.y && aMaxExtents.y >= bMinExtents.y) &&
					(aMinExtents.z <= bMaxExtents.z && aMaxExtents.z >= bMinExtents.z);

	if (collisionInfo.wasCollision)
	{
		// this normal is no work
		glm::vec3 interceptOne = aMinExtents - bMaxExtents;
		glm::vec3 interceptTwo = aMaxExtents - bMinExtents;

		glm::vec3 smallestIntercept;
		smallestIntercept.x = (abs(interceptOne.x) < abs(interceptTwo.x)) ? interceptOne.x : interceptTwo.x;
		smallestIntercept.y = (abs(interceptOne.y) < abs(interceptTwo.y)) ? interceptOne.y : interceptTwo.y;
		smallestIntercept.z = (abs(interceptOne.z) < abs(interceptTwo.z)) ? interceptOne.z : interceptTwo.z;

		float actualSmallest = smallestIntercept.x;
		glm::vec3 normal = glm::vec3(1, 0, 0);

		if (abs(smallestIntercept.y) < abs(actualSmallest))
		{
			actualSmallest = smallestIntercept.y;
			normal = glm::vec3(0, 1, 0);
		}
		if (abs(smallestIntercept.z) < abs(actualSmallest))
		{
			actualSmallest = smallestIntercept.z;
			normal = glm::vec3(0, 0, 1);
		}

		collisionInfo.interceptDistance = actualSmallest;
		collisionInfo.normal = normal;

	}
	




	//
	//float minDot;
	//float maxDot;
	//
	////float minDot = glm::dot(minExtents, glm::vec3(glm::vec3(plane->GetNormal()))) - plane->GetDistanceOrigin();
	////float maxDot = glm::dot(maxExtents, glm::vec3(glm::vec3(plane->GetNormal()))) - plane->GetDistanceOrigin();
	//
	//float dotToUse;
	//if (minDot < maxDot)
	//	dotToUse = minDot;
	//else
	//	dotToUse = maxDot;
	//
	//if (dotToUse <= 0)
	//{
	//	//we have a collision
	//	collisionInfo.normal = glm::vec3(plane->GetNormal());
	//	collisionInfo.interceptDistance = -dotToUse;
	//
	//	collisionInfo.wasCollision = true;
	//}
	//else
	//{
	//	collisionInfo.wasCollision = false;
	//}


	return collisionInfo.wasCollision;
}

bool PhysicCollision::CheckAABBPlane(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	bool wasCollision = CheckPlaneAABB(obj2, obj1, collisionInfo);
	if (wasCollision)
	{

		collisionInfo.normal = -collisionInfo.normal;
	}
	return wasCollision;

}

bool PhysicCollision::CheckSphereABBB(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	bool wasCollision = CheckAABBSphere(obj2, obj1, collisionInfo);
	if (wasCollision)
	{

		collisionInfo.normal = -collisionInfo.normal;
	}
	return wasCollision;
}

bool PhysicCollision::CheckAABBSphere(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	PhysicsAABBShape* AABB = (PhysicsAABBShape*)obj1->GetShape();
	PhysicsSphereShape* Sphere = (PhysicsSphereShape*)obj1->GetShape();

	// min max extents
	glm::vec3 aMin = glm::vec3(obj1->GetPosition()) - AABB->GetExtents();
	glm::vec3 aMax = glm::vec3(obj1->GetPosition()) + AABB->GetExtents();

	glm::vec3 bSpherePos = glm::vec3(obj2->GetPosition());
	// clamp the points
	float x = std::max(aMin.x, std::min(bSpherePos.x, aMax.x));
	float y = std::max(aMin.y, std::min(bSpherePos.y, aMax.y));
	float z = std::max(aMin.z, std::min(bSpherePos.z, aMax.z));

	float distance = glm::length(bSpherePos - glm::vec3(x, y, z));

	if (distance < Sphere->GetRadius())
	{

		collisionInfo.wasCollision = true;
		collisionInfo.normal = glm::normalize(glm::vec3(obj2->GetPosition() - obj1->GetPosition()));
		collisionInfo.interceptDistance = Sphere->GetRadius() - distance;
	}
	else
	{
		collisionInfo.wasCollision = false;
	}

	return collisionInfo.wasCollision;
}

bool PhysicCollision::PlaneToPlane(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	return false;
}

bool PhysicCollision::CheckPlaneAABB(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	PhysicsPlaneShape* plane = (PhysicsPlaneShape*)obj1->GetShape();
	PhysicsAABBShape* AABB = (PhysicsAABBShape*)obj2->GetShape();


	glm::vec3 minExtents((glm::vec3(obj2->GetPosition()) - AABB->GetExtents()));
	glm::vec3 maxExtents((glm::vec3(obj2->GetPosition()) + AABB->GetExtents()));

	float minDot = glm::dot(minExtents, glm::vec3(glm::vec3(plane->GetNormal()))) - plane->GetDistanceOrigin();
	float maxDot = glm::dot(maxExtents, glm::vec3(glm::vec3(plane->GetNormal()))) - plane->GetDistanceOrigin();

	float dotToUse;
	if (minDot < maxDot)
		dotToUse = minDot;
	else
		dotToUse = maxDot;

	if (dotToUse <= 0)
	{
		//we have a collision
		collisionInfo.normal = glm::vec3(plane->GetNormal());
		collisionInfo.interceptDistance = -dotToUse;

		collisionInfo.wasCollision = true;
	}
	else
	{
		collisionInfo.wasCollision = false;
	}


	return collisionInfo.wasCollision;
}

void PhysicCollision::ResolveCollision(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	if (collisionInfo.wasCollision)
	{
		HandleSeperation(obj1, obj2, collisionInfo);
		HandleVelocityChange(obj1, obj2, collisionInfo);
	}
}

void PhysicCollision::HandleSeperation(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	float totalSystemMass = obj1->GetMass() + obj2->GetMass();
	float obj1Offset = collisionInfo.interceptDistance * (obj2->GetMass() / totalSystemMass);
	float obj2Offset = collisionInfo.interceptDistance * (obj1->GetMass() / totalSystemMass);

	glm::vec3 obj1NewPosition = obj1->GetPosition() - (obj1Offset * collisionInfo.normal);
	glm::vec3 obj2NewPosition = obj2->GetPosition() + (obj2Offset * collisionInfo.normal);

	obj1->SetPosition(obj1NewPosition);
	obj2->SetPosition(obj2NewPosition);
}

void PhysicCollision::HandleVelocityChange(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	float e = 0.7f;
	glm::vec3 relativeVelocity = obj2->GetVelocity() - obj1->GetVelocity();

	float jTop = -(1 + e) * glm::dot(relativeVelocity, collisionInfo.normal);
	float jBottom = glm::dot(collisionInfo.normal, collisionInfo.normal) * (1.0f / obj1->GetMass() + 1.0f / obj2->GetMass());
	float j = jTop / jBottom;

	glm::vec3 obj1NewVelocity = obj1->GetVelocity() - ((j / obj1->GetMass()) * collisionInfo.normal);
	glm::vec3 obj2NewVelocity = obj2->GetVelocity() + ((j / obj2->GetMass()) * collisionInfo.normal);

	obj1->SetVelocity(obj1NewVelocity);
	obj2->SetVelocity(obj2NewVelocity);

}
