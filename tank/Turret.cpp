#include "Turret.h"
#include "Tank.h"

//---------------------------------------------------------------------------
Turret::Turret(Ogre::SceneManager* sceneMgr, Tank *tank, Physics *physicsEngine)
	: Entity(TANK_TURRET_MESH, sceneMgr, tank, physicsEngine), mTank(tank)
{
	auto tankHeight = tank->getHeight();
	auto turretHeight = Turret::getHeight();
	mSceneNode->setPosition(Ogre::Vector3(0, (tankHeight + turretHeight) / 2, 0));
}
//---------------------------------------------------------------------------
Ogre::Real Turret::getHeight() const
{
	return mEntity->getBoundingBox().getSize().y;
}
//---------------------------------------------------------------------------
Ogre::Real Turret::getWidth() const
{
	return mEntity->getBoundingBox().getSize().z;
}
//---------------------------------------------------------------------------
Ogre::Real Turret::getLength() const
{
	return mEntity->getBoundingBox().getSize().x;
}
//---------------------------------------------------------------------------
Turret::~Turret()
{
}
//---------------------------------------------------------------------------
btScalar Turret::getHalfHeight() const
{
	return halfHeight;
}
//---------------------------------------------------------------------------
btScalar Turret::getHalfWidth() const
{
	return halfWidth;
}
//---------------------------------------------------------------------------
btScalar Turret::getHalfLen() const
{
	return halfLen;
}
Tank* Turret::getTank()
{
	return mTank;
}

