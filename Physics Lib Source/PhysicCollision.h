#pragma once
#include "PhysicsObject.h"


#include "PhysicsAABBShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsPlaneShape.h"

#include <glm\glm.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace PhysicCollision
{
	struct CollisionInfo
	{
		bool		wasCollision;
		glm::vec3	normal;
		float		interceptDistance;
	};
	bool CheckCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	
	bool PlaneToPlane(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	bool CheckPlaneAABB(const PhysicsObject * shape1, const PhysicsObject * shape2, CollisionInfo & collisionInfo);
	bool CheckPlaneSphere(const PhysicsObject* pPlane, const PhysicsObject* pSphere, CollisionInfo& collisionInfo);

	bool CheckAABBPlane(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	bool CheckAABBAABB(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	bool CheckAABBSphere(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	
	bool CheckSpherePlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	bool CheckSphereABBB(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	bool CheckSphereSphereCollision(const PhysicsObject* pShere1, const PhysicsObject* pSphere2, CollisionInfo& collisionInfo);

	void ResolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleSeperation(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleVelocityChange(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);



};

