#include "Missle.h"
#include "Gun.h"
#include "World.h"
#include "InputDispatcher.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>

//---------------------------------------------------------------------------
Missle::Missle(Ogre::SceneManager* sceneMgr, Entity* gun, Physics *physicsEngine)
	: Entity(MISSLE_MESH, sceneMgr, gun, physicsEngine), mGun(nullptr), mBody(nullptr)
{
	mType = ENTITY_MISSLE;
	mGun = static_cast<Gun *>(gun);
	if (mGun)
	{
		mSceneNode->scale(2.5, 2.5, 2.5);
		makeParallel();

		//detach shell from gun, add it to world. Because after shell leaves the gun, it's position no longer related to gun's position.
		Ogre::Vector3 worldPos = mSceneNode->_getDerivedPosition();
		Ogre::Quaternion worldOri = mSceneNode->_getDerivedOrientation();

		detachSceneNode();
		mSceneNode->pitch(Ogre::Radian(180), Ogre::Node::TS_WORLD);

		// ensure the shell is parallel to the gun
		//create the new shape, and tell the physics that is a Box
		btCollisionShape *newRigidShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
		mBody = PhysicsSetup(newRigidShape, 0.5f, 0.1f, vectorBullet(&worldPos),
			quanBullet(&worldOri));
		Ogre::Vector3 velocity = worldOri * Ogre::Vector3(0, 1, 0) * MISSLE_VELOCITY;

		mBody->setLinearVelocity(*vectorBullet(&velocity));
	}
}
//---------------------------------------------------------------------------
Missle::~Missle()
{
}
//---------------------------------------------------------------------------
Ogre::Real Missle::getHeight() const
{
	return 0;
}
//---------------------------------------------------------------------------
Ogre::Real Missle::getWidth() const
{
	return 0;
}
//---------------------------------------------------------------------------
Ogre::Real Missle::getLength() const
{
	return 0;
}
//---------------------------------------------------------------------------
void Missle::update(float time)
{
}

//---------------------------------------------------------------------------
void Missle::collisionHandler(const btVector3* ptA, const btVector3* ptB, const btVector3* normalOnB, Entity* thatNode, std::list<Entity*>* list)
{
	if (thatNode && thatNode->getType() == ENTITY_AI_TANK)
	{
		list->push_back(this);
	}
}
void Missle::makeParallel() const
{
	auto gunLen = mGun->getLength();
	// Since the position of the gun has a margin of error, need to adjust the shell's position with it
	mSceneNode->translate(Ogre::Vector3(0.2, gunLen+0.5, 0), Ogre::Node::TS_LOCAL);

	auto gunOri = mGun->getSceneNode()->_getDerivedOrientation();
	mSceneNode->_setDerivedOrientation(gunOri);
}
