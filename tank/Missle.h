#pragma once

#include "Entity.h"
#include "World.h"
#include "InputListener.h"

class Gun;

static const std::string MISSLE_MESH = "Missle.mesh";
static const int MISSLE_VELOCITY = 200;

class Missle : public Entity
{
public:

	Missle(Ogre::SceneManager* sceneMgr, Entity* parent, Physics *physicsEngine);

	virtual ~Missle();

	Ogre::Real getHeight() const override;

	Ogre::Real getWidth() const override;

	Ogre::Real getLength() const override;

	void update(float time) override;

	void collisionHandler(const btVector3* ptA, const btVector3* ptB, const btVector3* normalOnB, Entity* thatNode, std::list<Entity*>* list) override;

protected:

	Entity *mGun;

	btRigidBody *mBody;

	void makeParallel() const;
};