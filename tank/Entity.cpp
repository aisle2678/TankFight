#include "Entity.h"
#include "Physics.h"
#include "MyMotionState.h"

Ogre::SceneNode* Entity::getSceneNode() const
{
	return mSceneNode;
}

Ogre::SceneNode* Entity::getExplosionSceneNode() const
{
	return explosionSceneNode;
}


Ogre::Entity* Entity::getEntity() const
{
	return mEntity;
}

Ogre::SceneManager* Entity::getSceneManager() const
{
	return mSceneMgr;
}

int Entity::getType() const
{
	return mType;
}

void Entity::setType(int type)
{
	mType = type;
}

void Entity::update(float time)
{
}

Physics* Entity::getPhysicsEngine() {
	return physicsEngine;
}

btRigidBody* Entity::getPhysicsEngineEntity() {
	return physicsEngineEntity;
}

void Entity::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list) {
}

Entity::Entity(const std::string& name, Ogre::SceneManager *sceneMgr, Entity *parent, Physics *physicsEngine)
	: mSceneMgr(sceneMgr), mSceneNode(nullptr), mEntity(nullptr), physicsEngine(physicsEngine), physicsEngineEntity(nullptr)
{
	mType = 0;

	explosionSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	if (parent)
	{
		mSceneNode = parent->getSceneNode()->createChildSceneNode();	
	}
	else
	{
		mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	}

	if (!name.empty())
	{
		mEntity = mSceneMgr->createEntity(name);
		mSceneNode->attachObject(mEntity);
	}
}

Entity::~Entity()
{
}

void Entity::setPosition(Ogre::Vector3 &pos){
	btTransform transform;
	transform.setIdentity();
	//physicsEngineEntity->getMotionState()->getWorldTransform(transform);f
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	physicsEngineEntity->setWorldTransform(transform);
}

void Entity::detachSceneNode(){
	Ogre::Vector3 worldPos = mSceneNode->_getDerivedPosition();
	Ogre::Quaternion worldOri = mSceneNode->_getDerivedOrientation();

	mSceneMgr->destroySceneNode(mSceneNode);
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mSceneNode->attachObject(mEntity);
	mSceneNode->setPosition(worldPos);
	mSceneNode->setOrientation(worldOri);
}

void Entity::setOrientation(Ogre::Quaternion &ori){

}

btRigidBody* Entity::PhysicsSetup(btCollisionShape *shape, btScalar mass, btScalar friction, btVector3 *initPos, btQuaternion *ori) {
	physicsEngine->collisionShapes.push_back(shape);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0,0,0);
	startTransform.setOrigin(*initPos);
	if (ori) startTransform.setRotation(*ori);
	shape->calculateLocalInertia(mass, localInertia);

	btMotionState *motionState;
	if (mass == 0.0){
		motionState = new btDefaultMotionState(startTransform);
	}else{
		motionState = new MyMotionState(startTransform, mSceneNode);
	}

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setRestitution(1);
	body->setFriction(friction);

	physicsEngineEntity = body;
	body->setUserPointer(this);

	physicsEngine->addRigidBody(body);

	return body;
}

btVector3 *Entity::vectorBullet(Ogre::Vector3 *vector) {
	return new btVector3(vector->x, vector->y, vector->z);
}

Ogre::Vector3 *Entity::vectorOgre(btVector3 *vector) {
	return new Ogre::Vector3(vector->x(), vector->y(), vector->z());
}

btQuaternion *Entity::quanBullet(Ogre::Quaternion *quan) {
	return new btQuaternion(quan->x, quan->y, quan->z, quan->w);
}

Ogre::Quaternion *Entity::quanOgre(btQuaternion *quan) {
	return new Ogre::Quaternion(quan->w(), quan->x(), quan->y(), quan->z());
}
