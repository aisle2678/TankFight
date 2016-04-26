#include "AITank.h"
#include "Physics.h"
#include "World.h"
#include "PlayerTank.h"
#include "Turret.h"
#include "Gun.h"
#include "MiniMapManager.h"

int AITank::id = 0;
//---------------------------------------------------------------------------
AITank::AITank(Ogre::SceneManager* sceneMgr, World* w, Physics *physicEngine, std::string *aiTankId)
	: Tank(sceneMgr, w, physicEngine), mAccumultedTime(0), mLastChangeTime(0)
{
	id++;
	collidedWith = 0;
	shouldDelete = false;
	deleted = false;
	mTankId = aiTankId;
	mType = ENTITY_AI_TANK;

	miniMapSceneNode = w->getMiniMapMgr()->addMiniMapEntity(&Ogre::Vector3(0,0,0), MINI_MAP_MESH);
}
//---------------------------------------------------------------------------
AITank::~AITank()
{
}
//---------------------------------------------------------------------------
void AITank::setDeleteFlag(){
	shouldDelete = true;
}
//---------------------------------------------------------------------------
bool AITank::ShouldDelete(){
	return shouldDelete;
}
//---------------------------------------------------------------------------
bool AITank::alreadyDeleted(){
	return deleted;
}
//---------------------------------------------------------------------------
void AITank::think(float time)
{
	move(time);
	turretFollow(time);
	mTimeElapsed += time;
}
//---------------------------------------------------------------------------
std::string *AITank::getTankId() const
{
	return mTankId;
}
//---------------------------------------------------------------------------
void AITank::move(float time)
{
	auto pos = mSceneNode->getPosition();
	mWorld->getMiniMapMgr()->updateAI(&pos, miniMapSceneNode);

	if (mWorld->needEvasion(pos))
	{
		mRotation = TANK_ROTATION_SPEED;
		mAccumultedTime = 0;
	}
	else
	{
		mDirection.x = -TANK_INITIAL_SPEED;
		mRotation = 0;
	}

	auto worldOri = mSceneNode->_getDerivedOrientation();
	auto velocity = worldOri * Ogre::Vector3(TANK_INITIAL_SPEED, 0, 0) * mDirection.x;
	this->physicsEngineEntity->setLinearVelocity(btVector3(velocity.x, 0, velocity.z));
	this->physicsEngineEntity->setAngularVelocity(btVector3(0, mRotation, 0));

}
//---------------------------------------------------------------------------
void AITank::turretFollow(float time)
{
	// get the vector pointing from AI to player.
	auto player = mWorld->getPlayerTank();
	auto playerPos = player->getSceneNode()->getPosition();
	auto aiPos = mSceneNode->getPosition();
	auto aiToPlayer = playerPos - aiPos;

	if (aiToPlayer.length() < VIGILENT_DISTANCE)
	{
		// the direction vector of the turret.
		auto turretOri = mTurret->getSceneNode()->_getDerivedOrientation();
		auto turretDirection = turretOri * Ogre::Vector3::NEGATIVE_UNIT_X;
		// important: cancel out the y rotation!
		aiToPlayer.y = 0;

		auto rotation = turretDirection.getRotationTo(aiToPlayer);
		rotation.x = 0;
		rotation.z = 0;
		mTurret->getSceneNode()->rotate(rotation);

		// boommmmm
		shootShell();
	} 
}
//---------------------------------------------------------------------------
void AITank::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list){
	if (!thatNode) return;
	if (thatNode->getType() == ENTITY_SHELL_FROM_PLAYER){
		collidedWith |= ENTITY_SHELL_FROM_PLAYER;
		list->push_back(this);
	}else if (thatNode->getType() == ENTITY_MINE){
		collidedWith |= ENTITY_MINE;
		list->push_back(this);
	}else if (thatNode->getType() == ENTITY_MISSLE){
		collidedWith |= ENTITY_MISSLE;
		list->push_back(this);
	}
}
//---------------------------------------------------------------------------
void AITank::Explode() {
	if (deleted) return;

	mWorld->getMiniMapMgr()->deleteEntity(miniMapSceneNode);

	if (collidedWith & ENTITY_MINE){
		explodedForMine();
	}else if (collidedWith & ENTITY_MISSLE){
		explodedForMissle();
	}else if (collidedWith & ENTITY_SHELL_FROM_PLAYER){
		explodedForShell();
	}
	Ogre::ParticleSystem *ps = mSceneMgr->createParticleSystem("AITANK" + std::to_string(id++), "Examples/Smoke");
	mSceneNode->attachObject(ps);
	deleted = true;
}

void AITank::explodedForMine(){
	// randomly typed some random range :)
	btScalar randomVec1 = Ogre::Math::RangeRandom(-10, 10);
	btScalar randomVec2 = Ogre::Math::RangeRandom(-2, 2);
	btScalar randomVec3 = Ogre::Math::RangeRandom(-4, 6);
	btScalar randomVec4 = Ogre::Math::RangeRandom(-7, 8);
	btScalar randomVec5 = Ogre::Math::RangeRandom(10, 20);
	btScalar randomVec6 = Ogre::Math::RangeRandom(1, 9);

	this->physicsEngineEntity->setAngularFactor(1);
	this->physicsEngineEntity->setAngularVelocity(btVector3(randomVec1, randomVec2, randomVec3));
	this->physicsEngineEntity->setLinearVelocity(btVector3(randomVec4, randomVec5, randomVec6));

	mGun->detachSceneNode();
	Ogre::SceneNode *gunSceneNode = mGun->getSceneNode();
	Ogre::Vector3 gunPos = gunSceneNode->_getDerivedPosition();
	Ogre::Quaternion gunOri = gunSceneNode->_getDerivedOrientation();
	btCollisionShape *gunShape = new btBoxShape(btVector3(0.2f, 1.4f, 0.2f));
	btRigidBody* gunBody = mGun->PhysicsSetup(gunShape, 10.f, 5.f, vectorBullet(&gunPos), quanBullet(&gunOri));
	gunBody->setLinearVelocity(btVector3(randomVec1, randomVec6, randomVec3));
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
	turretBody->setLinearVelocity(btVector3(randomVec4, randomVec3, randomVec6));
	turretBody->setAngularVelocity(btVector3(randomVec4, randomVec5, randomVec6));
}

void AITank::explodedForMissle(){
	explodedForShell();
}

