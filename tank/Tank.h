#pragma once

#include "Entity.h"
#include "InputListener.h"

class TankCamera;
class Turret;
class Gun;
class Physics;
class MiniMapEntity;

const std::string TANK_BODY_MESH = "TankBody.mesh";
const Ogre::Real TANK_INITIAL_SPEED = Ogre::Real(5.f);
const Ogre::Real TANK_ROTATION_SPEED = Ogre::Real(2.f);

class Tank : public Entity, public InputListener
{
public:

	const static float LOAD_TIME;

	Tank(Ogre::SceneManager *sceneMgr, World *w, Physics *physicsEngine);

	virtual ~Tank();

	Ogre::Real getHeight() const override;

	Ogre::Real getWidth() const override;

	Ogre::Real getLength() const override;

	/*get parameters of physics engine body*/
	btScalar getHalfHeight() const;
	
	btScalar getHalfWidth() const;
	
	btScalar getHalfLen() const;

	void setWorld(World *w);

	World *getWorld() const;

	Turret *getTurret() const;

	int getLife() const;

	void reduceLife();

	void increaseLife(int life);

	virtual void collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list) override;

protected:

	void initializeTank();

	void shootShell();

	void explodedForShell();

	// tank owns turret and gun
	std::unique_ptr<Turret> mTurret;
	std::unique_ptr<Gun> mGun;

	// tank should know about the world
	World *mWorld;

	Ogre::Vector3 mDirection;
	Ogre::Real mRotation;
	
	float mTimeElapsed;

	btScalar halfHeight;
	btScalar halfLen;
	btScalar halfWidth;

	int mLife;
};
