#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Collision.h"
#include "PhysicCollision.h"
PhysicsScene::PhysicsScene()
{
}


PhysicsScene::~PhysicsScene()
{
	for (PhysicsObject* obj : m_physicsObjects)
	{
		delete obj;
	}
}

void PhysicsScene::Add(PhysicsObject * newObject)
{
	m_physicsObjects.insert(newObject);
}

void PhysicsScene::Remove(PhysicsObject * objectToRemove)
{
	// check if the object is infact inside the set.
	auto it = m_physicsObjects.find(objectToRemove);
	if (it != m_physicsObjects.end())
	{
		// delete the object and remove the index.
		delete *it;
		m_physicsObjects.erase(it);
	}

}

void PhysicsScene::Update(float deltaTime)
{
	if (deltaTime > 0.3) deltaTime = 0.3f;

	for (PhysicsObject* object : m_physicsObjects)
	{
		if (!object->GetIsStatic())
		{
 			object->AddAcceleration(m_gravity);
			object->Update(deltaTime);
		}
	}

	PhysicCollision::CollisionInfo collinfo;
	for (auto objIteratorOne = m_physicsObjects.begin(); objIteratorOne != m_physicsObjects.end(); objIteratorOne++)
	{
		for (auto objIteratorTwo = std::next(objIteratorOne); objIteratorTwo != m_physicsObjects.end(); objIteratorTwo++)
		{
			bool wasCollision = PhysicCollision::CheckCollision(*objIteratorOne, *objIteratorTwo, collinfo);
			if (wasCollision)
			{
				PhysicCollision::ResolveCollision(*objIteratorOne, *objIteratorTwo, collinfo);
			}
		}
	}
	
}

void PhysicsScene::SetGravity(const glm::vec3& gravity)
{
	m_gravity = gravity;
}

glm::vec3 PhysicsScene::GetGravity() const
{
	return m_gravity;
}
