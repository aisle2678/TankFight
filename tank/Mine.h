#pragma once

#include "Entity.h"

static const std::string MINE_MESH = "Mine.mesh";

class Mine : public Entity
{
public:
	Mine(Ogre::SceneManager* sceneMgr, Entity* parent, Physics *physicsEngine);

	Ogre::Real getHeight() const override;

	Ogre::Real getWidth() const override;

	Ogre::Real getLength() const override;

	void collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list);
};