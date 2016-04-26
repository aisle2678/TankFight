#pragma once

#include "Tank.h"

static const Ogre::Real VIGILENT_DISTANCE = 100;

static const Ogre::String MINI_MAP_MESH = "MiniMapAIEntity.mesh";

class Physics;

class AITank : public Tank
{
public:
	
	AITank(Ogre::SceneManager* sceneMgr, World* w, Physics *physicEngine, std::string *aiTankId);

	~AITank();

	void think(float time);

	std::string *getTankId() const;

	void setDeleteFlag();

	bool ShouldDelete();

	void collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list);

	void Explode();

	bool alreadyDeleted();

	//void setPosition(const Ogre::Vector3 &pos);
	static int id;

protected:

	void move(float time);

	void turretFollow(float time);

	void explodedForMine();

	void explodedForMissle();

	float mAccumultedTime;

	float mLastChangeTime;

	std::string *mTankId;

	bool shouldDelete;

	bool deleted;

	int collidedWith;

	Ogre::SceneNode* miniMapSceneNode;
};
