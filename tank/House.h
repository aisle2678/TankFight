#pragma once

#include "Entity.h"

const static Ogre::Real SCALE_FACTOR = 0.02;
const static std::string HOUSE_MESH = "tudorhouse.mesh";

class House : public Entity
{
public:

	House(Ogre::SceneManager* sceneMgr, Entity* parent, Physics* physicsEngine, Ogre::Vector3 *pos);

	Ogre::Real getHeight() const override;

	Ogre::Real getWidth() const override;

	Ogre::Real getLength() const override;

	void House::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list) override;
};
