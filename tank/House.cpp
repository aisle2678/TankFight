#include "House.h"

//---------------------------------------------------------------------------
House::House(Ogre::SceneManager* sceneMgr, Entity* parent, Physics *physicsEngine, Ogre::Vector3 *pos)
	: Entity(HOUSE_MESH, sceneMgr, parent, physicsEngine)
{
	mType = ENTITY_HOUSE;
	mSceneNode->scale(0.02f, 0.02f, 0.02f);
	mSceneNode->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 360)));
	mSceneNode->setPosition(*pos);
	Ogre::Vector3 worldPos = mSceneNode->_getDerivedPosition();
	Ogre::Quaternion worldOri = mSceneNode->_getDerivedOrientation();
	btCollisionShape *shape = new btBoxShape(btVector3(8.f, 10.f, 8.f));
	this->PhysicsSetup(shape, 0., 0., vectorBullet(pos), quanBullet(&worldOri));
}
//---------------------------------------------------------------------------
Ogre::Real House::getHeight() const
{
	return 0;
}
//---------------------------------------------------------------------------
Ogre::Real House::getWidth() const
{
	return 0;
}
//---------------------------------------------------------------------------
Ogre::Real House::getLength() const
{
	return 0;
}
void House::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list){
	if (!thatNode) return;
}
