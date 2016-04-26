#include "Gun.h"
#include "Shell.h"
#include "Physics.h"
#include "Turret.h"
#include "Tank.h"

int Shell::id = 0;
//---------------------------------------------------------------------------
Shell::Shell(Ogre::SceneManager* sceneMgr, Gun *gun, Physics *physicsEngine)
	: Entity(TANK_SHELL_MESH, sceneMgr, gun, physicsEngine), mGun(gun)
{
	// ensure the shell is parallel to the gun
	makeParallel();

	//detach shell from gun, add it to world. Because after shell leaves the gun, it's position no longer related to gun's position.
	Ogre::Vector3 worldPos = mSceneNode->_getDerivedPosition();
	Ogre::Quaternion worldOri = mSceneNode->_getDerivedOrientation();
	Ogre::Quaternion convertedOri = Ogre::Quaternion(worldOri.w, worldOri.x, worldOri.y, worldOri.z);
	Ogre::Vector3 velocity = convertedOri * Ogre::Vector3(0, 1, 0) * SHELL_VELOCITY;

	detachSceneNode();
	mSceneNode->pitch(Ogre::Radian(180), Ogre::Node::TS_WORLD);

	//create the new shape, and tell the physics that is a Box
	btCollisionShape *newRigidShape = new btBoxShape(btVector3(.1f, .1f, .1f));
	btRigidBody* body = PhysicsSetup(newRigidShape, 1.f, 1.f, vectorBullet(&worldPos),
		quanBullet(&worldOri));

	body->setLinearVelocity(*vectorBullet(&velocity) + mGun->getTurret()->getTank()->getPhysicsEngineEntity()->getLinearVelocity());

	Ogre::ParticleSystem *ps2 = mSceneMgr->createParticleSystem("shellFire" + std::to_string(id++), "Examples/GreenyNimbus");
	mSceneNode->attachObject(ps2);
}

void Shell::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list){
	list->push_back(this);
	if (thatNode)
	{
		thatNode->getExplosionSceneNode()->_setDerivedPosition(*vectorOgre(&btVector3(ptA->x(), ptA->y(), ptA->z())));
		Ogre::ParticleSystem *ps = mSceneMgr->createParticleSystem("ExplodeWhenAttacked" + std::to_string(id++), "explosionWhenAttacked");
		thatNode->getExplosionSceneNode()->detachAllObjects();
		thatNode->getExplosionSceneNode()->attachObject(ps);
	}
	
}

//---------------------------------------------------------------------------
Ogre::Real Shell::getHeight() const
{
	return mEntity->getBoundingBox().getSize().x;
}
//---------------------------------------------------------------------------
Ogre::Real Shell::getWidth() const
{
	return mEntity->getBoundingBox().getSize().z;
}
//---------------------------------------------------------------------------
Ogre::Real Shell::getLength() const
{
	return mEntity->getBoundingBox().getSize().y;
}
//---------------------------------------------------------------------------
Shell::~Shell()
{
}
//---------------------------------------------------------------------------
void Shell::makeParallel() const
{
	auto gunLen = mGun->getLength();
	// Since the position of the gun has a margin of error, need to adjust the shell's position with it
	mSceneNode->translate(Ogre::Vector3(0.2, gunLen+0.5, 0), Ogre::Node::TS_LOCAL);

	auto gunOri = mGun->getSceneNode()->_getDerivedOrientation();
	mSceneNode->_setDerivedOrientation(gunOri);
}
