#pragma once

#include "btBulletCollisionCommon.h"

class Physics;

// Abstract calss for entities in the world.
class Entity
{
public:
	enum EntityTypes {
		ENTITY_AI_TANK = 1,
		ENTITY_SHELL_FROM_AI = 2,
		ENTITY_PLAYER_TANK = 4,
		ENTITY_SHELL_FROM_PLAYER = 8,
		ENTITY_HOUSE = 16,
		ENTITY_MISSLE = 32,
		ENTITY_MINE = 64,
		ENTITY_WALL = 128
	};

	Ogre::SceneNode *getSceneNode() const;

	Ogre::SceneNode *getExplosionSceneNode() const;
	
	Ogre::Entity *getEntity() const;

	Ogre::SceneManager *getSceneManager() const;

	int getType() const;

	btRigidBody* Entity::PhysicsSetup(btCollisionShape *shape, btScalar mass, btScalar frication, btVector3 *initPos, btQuaternion *ori);

	static btVector3 *vectorBullet(Ogre::Vector3 *vector);

	static Ogre::Vector3 *vectorOgre(btVector3 *vector);

	static btQuaternion *quanBullet(Ogre::Quaternion *quan);

	static Ogre::Quaternion *quanOgre(btQuaternion *quan);

	void setType(int type);

	void detachSceneNode();

	virtual Ogre::Real getHeight() const = 0;

	virtual Ogre::Real getWidth() const = 0;

	virtual Ogre::Real getLength() const = 0;

	virtual Physics *getPhysicsEngine();

	virtual btRigidBody *getPhysicsEngineEntity();

	virtual void update(float time);

	virtual ~Entity();

	virtual void setPosition(Ogre::Vector3 &pos);

	virtual void setOrientation(Ogre::Quaternion &ori);

	virtual void collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list);

protected:
	Entity(const std::string &name, Ogre::SceneManager *sceneMgr, Entity *parent, Physics *physicsEngine); 

	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mSceneNode;
	Ogre::Entity *mEntity;
	Physics *physicsEngine;
	btRigidBody *physicsEngineEntity;
	int mType;
	Ogre::SceneNode *explosionSceneNode;
};