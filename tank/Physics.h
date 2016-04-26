#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class Physics
{
public:
	void initObjects();
	void addRigidBody(btRigidBody *body);
	void deleteRigidBody(btRigidBody *body);
	int getCollisionObjectCount();
	btDiscreteDynamicsWorld *getDynamicsWorld();

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;

protected:
	int rigidBodyCount;
	btDiscreteDynamicsWorld* dynamicsWorld;
};