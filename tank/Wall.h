#pragma once

#include "Entity.h"

class Gun;
class Physics;

class Wall : public Entity
{
public:
	Wall(Ogre::SceneManager *sceneMgr, Physics *physicEngine, btVector3* origin);

	void collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list) override;

	~Wall();

	Ogre::Real getHeight() const override;

	Ogre::Real getWidth() const override;

	Ogre::Real getLength() const override;
};