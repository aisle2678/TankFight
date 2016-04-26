#pragma once

#include "Entity.h"

class Turret;
class Physics;

const std::string TANK_GUN_MESH = "TankGun.mesh";
const Ogre::Real PITCH_SPEED = .08f;

class Gun : public Entity
{
public:
	explicit Gun(Ogre::SceneManager *sceneMgr, Turret *turret, Physics *physicEngine);

	virtual Ogre::Real getHeight() const override;

	virtual Ogre::Real getWidth() const override;

	virtual Ogre::Real getLength() const override;

	virtual ~Gun();

	Turret* getTurret();

protected:
	Turret *mTurret;
};