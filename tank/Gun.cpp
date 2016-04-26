#include "Gun.h"
#include "Turret.h"
#include "Physics.h"

//---------------------------------------------------------------------------
Gun::Gun(Ogre::SceneManager* sceneMgr, Turret *turret, Physics *physicsEngine)
	: Entity(TANK_GUN_MESH, sceneMgr, turret, physicsEngine), mTurret(turret)
{
	mSceneNode->rotate(Ogre::Vector3(0,1,0), Ogre::Degree(90));
	mSceneNode->rotate(Ogre::Vector3(1,0,0), Ogre::Degree(-90));
	auto turretLen=  turret->getLength();
	mSceneNode->translate(Ogre::Vector3(-turretLen / 2, 0, 0));
}
//---------------------------------------------------------------------------
Ogre::Real Gun::getHeight() const
{
	return mEntity->getBoundingBox().getSize().x;
}
//---------------------------------------------------------------------------
Ogre::Real Gun::getWidth() const
{
	return mEntity->getBoundingBox().getSize().z;
}
//---------------------------------------------------------------------------
Ogre::Real Gun::getLength() const
{
	return mEntity->getBoundingBox().getSize().y;
}
//---------------------------------------------------------------------------
Gun::~Gun()
{
	
}
Turret* Gun::getTurret()
{
	return mTurret;
}

