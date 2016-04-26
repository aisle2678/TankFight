#pragma once

#include "Entity.h"

class Tank;
class Physics;

const std::string TANK_TURRET_MESH = "TankTurret.mesh";
const Ogre::Real YAW_SPEED = .05f;

class Turret : public Entity
{
public:
	explicit Turret(Ogre::SceneManager *sceneMgr, Tank *tank, Physics *physicsEngine);

	Ogre::Real getHeight() const override;

	Ogre::Real getWidth() const override;

	Ogre::Real getLength() const override;

	virtual ~Turret();

	/*get parameters of physics engine body*/
	btScalar getHalfHeight() const;
	
	btScalar getHalfWidth() const;
	
	btScalar getHalfLen() const;

	Tank* getTank();

protected:
	btScalar halfHeight;
	btScalar halfLen;
	btScalar halfWidth;
	Tank* mTank;
};
