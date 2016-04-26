#include "Physics.h"

void Physics::initObjects() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	rigidBodyCount = 0;
}

void Physics::addRigidBody(btRigidBody *body) {
	this->dynamicsWorld->addRigidBody(body);
	rigidBodyCount++;
}

void Physics::deleteRigidBody(btRigidBody *body) {
	if (body && body->getMotionState()){
		delete body->getMotionState();
	}
	this->dynamicsWorld->removeCollisionObject(body);
	delete body;
	rigidBodyCount--;
}

int Physics::getCollisionObjectCount() {
	return this->rigidBodyCount;
}

btDiscreteDynamicsWorld *Physics::getDynamicsWorld() {
	return this->dynamicsWorld;
}