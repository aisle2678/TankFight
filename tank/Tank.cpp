#include "Tank.h"
#include "Turret.h"
#include "Gun.h"
#include "World.h"
#include "Shell.h"

//---------------------------------------------------------------------------
const float Tank::LOAD_TIME = 1.5;
//---------------------------------------------------------------------------
Tank::Tank(Ogre::SceneManager* sceneMgr, World *w, Physics *physicsEngine)
	: Entity(TANK_BODY_MESH, sceneMgr, nullptr, physicsEngine), 
	mWorld(w), mDirection(Ogre::Vector3(0, 0, 0)), mTimeElapsed(.0f), mLife(1)
{
	initializeTank();
	mRotation = 0;

	// create the new shape, and tell the physics that is a Box
	halfHeight = 1.5f;
	halfWidth = 2.0f;
	halfLen = 2.8f;
	btCollisionShape *newRigidShape = new btBoxShape(btVector3(halfLen, halfHeight, halfWidth));
	btRigidBody *body = this->PhysicsSetup(newRigidShape, 300.0f, 5.f, &btVector3(0, -1., 0), nullptr);
	body->setAngularFactor(0);  //this disable rotation when collision happens.
}
void Tank::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list){
	if (!thatNode) return;
	if ((mType == ENTITY_AI_TANK) && (thatNode->getType() == ENTITY_SHELL_FROM_PLAYER)) list->push_back(this);
}
//---------------------------------------------------------------------------
Tank::~Tank()
{
}
//---------------------------------------------------------------------------
Ogre::Real Tank::getWidth() const
{
	return mEntity->getBoundingBox().getSize().z;
}
//---------------------------------------------------------------------------
Ogre::Real Tank::getHeight() const
{
	return mEntity->getBoundingBox().getSize().y;
}
//---------------------------------------------------------------------------
Ogre::Real Tank::getLength() const
{
	return mEntity->getBoundingBox().getSize().x;
}
//---------------------------------------------------------------------------
btScalar Tank::getHalfHeight() const
{
	return halfHeight;
}
//---------------------------------------------------------------------------
btScalar Tank::getHalfWidth() const
{
	return halfWidth;
}
//---------------------------------------------------------------------------
btScalar Tank::getHalfLen() const
{
	return halfLen;
}
//---------------------------------------------------------------------------
void Tank::setWorld(World* w)
{
	mWorld = w;
}
//---------------------------------------------------------------------------
World* Tank::getWorld() const
{
	return mWorld;
}
//---------------------------------------------------------------------------
Turret* Tank::getTurret() const
{
	return mTurret.get();
}

//---------------------------------------------------------------------------
void Tank::initializeTank()
{
	mTurret = std::unique_ptr<Turret>(new Turret(mSceneMgr, this, this->getPhysicsEngine()));
	mGun = std::unique_ptr<Gun>(new Gun(mSceneMgr, mTurret.get(), this->getPhysicsEngine()));
}

//---------------------------------------------------------------------------
void Tank::shootShell()
{
	static int id = 0;
	if (mTimeElapsed > LOAD_TIME)
	{
		Shell* shell = new Shell(mSceneMgr, mGun.get(), this->getPhysicsEngine());
		if (mType == ENTITY_AI_TANK){
			shell->setType(ENTITY_SHELL_FROM_AI);
		}else {
			shell->setType(ENTITY_SHELL_FROM_PLAYER);
		}
		mTimeElapsed = 0.0f;

		Ogre::ParticleSystem *ps = mSceneMgr->createParticleSystem("ExploWhenFire" + std::to_string(id++), "explosionTemplate");
		mGun->getSceneNode()->attachObject(ps);
		mWorld->getSoundEngine()->play2D("Content/Sound/tank_shot.flac", false);
	}
}
//---------------------------------------------------------------------------
int Tank::getLife() const
{
	return mLife;
}
//---------------------------------------------------------------------------
void Tank::reduceLife()
{
	--mLife;
}
void Tank::increaseLife(int life)
{
	mLife += life;
}
void Tank::explodedForShell(){
	this->physicsEngineEntity->setAngularVelocity(btVector3(0, 0, 0));
	this->physicsEngineEntity->setFriction(100.f);

	// randomly typed some random range :)
	btScalar randomVec1 = Ogre::Math::RangeRandom(-10, 10);
	btScalar randomVec2 = Ogre::Math::RangeRandom(-2, 2);
	btScalar randomVec3 = Ogre::Math::RangeRandom(-4, 6);
	btScalar randomVec4 = Ogre::Math::RangeRandom(-7, 8);
	btScalar randomVec5 = Ogre::Math::RangeRandom(-10, 8);
	btScalar randomVec6 = Ogre::Math::RangeRandom(1, 9);

	mGun->detachSceneNode();
	Ogre::SceneNode *gunSceneNode = mGun->getSceneNode();
	Ogre::Vector3 gunPos = gunSceneNode->_getDerivedPosition();
	Ogre::Quaternion gunOri = gunSceneNode->_getDerivedOrientation();
	btCollisionShape *gunShape = new btBoxShape(btVector3(0.2f, 1.4f, 0.2f));
	btRigidBody* gunBody = mGun->PhysicsSetup(gunShape, 20.f, 5.f, vectorBullet(&gunPos), quanBullet(&gunOri));
	gunBody->setLinearVelocity(btVector3(randomVec1, randomVec2, randomVec3));
	gunBody->setAngularVelocity(btVector3(randomVec1, randomVec2, randomVec3));

	mTurret->detachSceneNode();
	Ogre::SceneNode *turretSceneNode = mTurret->getSceneNode();
	Ogre::Vector3 turretPos = turretSceneNode->_getDerivedPosition();
	Ogre::Quaternion turretOri = turretSceneNode->_getDerivedOrientation();
	halfHeight = 0.75f;
	halfWidth = 1.0f;
	halfLen = 1.4f;
	btCollisionShape *turretShape = new btBoxShape(btVector3(halfLen, halfHeight, halfWidth));
	btRigidBody* turretBody = mTurret->PhysicsSetup(turretShape, 100.f, 5.f, vectorBullet(&turretPos), quanBullet(&turretOri));
	turretBody->setLinearVelocity(btVector3(randomVec4, randomVec5, randomVec6));
	turretBody->setAngularVelocity(btVector3(randomVec4, randomVec5, randomVec6));
}
