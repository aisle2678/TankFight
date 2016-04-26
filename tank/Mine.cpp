#include "Mine.h"

//---------------------------------------------------------------------------
Mine::Mine(Ogre::SceneManager* sceneMgr, Entity* parent, Physics *physicsEngine)
	: Entity(MINE_MESH, sceneMgr, parent, physicsEngine)
{
	mType = ENTITY_MINE;
	mSceneNode->translate(Ogre::Vector3(5, -1, 0), Ogre::Node::TS_PARENT);

	Ogre::Vector3 worldPos = mSceneNode->_getDerivedPosition();
	Ogre::Quaternion worldOri = mSceneNode->_getDerivedOrientation();

	detachSceneNode();

	btCollisionShape *newRigidShape = new btBoxShape(btVector3(1.0f, 3.f, 1.0f));
	btRigidBody* body = PhysicsSetup(newRigidShape, 0.f, 0.f, vectorBullet(&worldPos),
		quanBullet(&worldOri));
}
void Mine::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list){
	if (thatNode->getType() == ENTITY_AI_TANK){
		list->push_back(this);
	}
}
//---------------------------------------------------------------------------
Ogre::Real Mine::getHeight() const
{
	return 0;
}
//---------------------------------------------------------------------------
Ogre::Real Mine::getWidth() const
{
	return 0;
}
//---------------------------------------------------------------------------
Ogre::Real Mine::getLength() const
{
	return 0;
}
