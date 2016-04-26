#pragma once

#include "Entity.h"

class Gun;
class Physics;

const std::string TANK_SHELL_MESH = "Shell.mesh";
const int SHELL_VELOCITY = 150;

class Shell : public Entity
{
public:

	explicit Shell(Ogre::SceneManager *sceneMgr, Gun *gun, Physics *physicEngine);

	virtual Ogre::Real getHeight() const override;

	virtual Ogre::Real getWidth() const override;

	virtual Ogre::Real getLength() const override;

	virtual void collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list) override;

	virtual ~Shell();

	static int id;

protected:

	void makeParallel() const;

	Gun *mGun;

};