#include "PlayerTank.h"
#include "Physics.h"
#include "World.h"
#include "Turret.h"
#include "TankCamera.h"
#include "Gun.h"
#include "Missle.h"
#include "Mine.h"
#include "MiniMapManager.h"

//---------------------------------------------------------------------------
PlayerTank::PlayerTank(Ogre::SceneManager* sceneMgr, World* w, Physics *physicEngine)
	: Tank(sceneMgr, w, physicEngine), mMisslesNum(0)
{
	dead = false;
	mType = ENTITY_PLAYER_TANK;
	btRigidBody *tankBody;
	tankBody = physicsEngineEntity;
	tankBody->setActivationState(DISABLE_DEACTIVATION);
	mLife = PLAYER_INITIAL_LIFE;

	Ogre::Vector3 pos = mSceneNode->_getDerivedPosition();
	w->getMiniMapMgr()->addPlayerTank(&pos);
}
//---------------------------------------------------------------------------
PlayerTank::~PlayerTank()
{
}
//---------------------------------------------------------------------------
void PlayerTank::keyPressed(const OIS::KeyEvent& ke)
{
	switch (ke.key)
	{
	case OIS::KC_W:
		mDirection.x = -TANK_INITIAL_SPEED;
		break;
	case OIS::KC_S:
		mDirection.x = TANK_INITIAL_SPEED;
		break;
	case OIS::KC_A:
		mRotation = TANK_ROTATION_SPEED;
		break;
	case OIS::KC_D:
		mRotation = -TANK_ROTATION_SPEED;
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
void PlayerTank::collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list){
	if (!thatNode) return;
	if (thatNode->getType() == ENTITY_SHELL_FROM_AI){
		mLife--;
	}
}
//---------------------------------------------------------------------------
void PlayerTank::keyReleased(const OIS::KeyEvent& ke)
{
	switch (ke.key)
	{
	case OIS::KC_W:
		mDirection.x = 0;
		break;
	case OIS::KC_S:
		mDirection.x = 0;
		break;
	case OIS::KC_A:
		mRotation = 0;
		break;
	case OIS::KC_D:
		mRotation = 0;
		break;
	case OIS::KC_SPACE:
		createMine();
	default:
		break;
	}
}
//---------------------------------------------------------------------------
void PlayerTank::mouseMoved(const OIS::MouseEvent& me)
{
	//mPlayerCamera->getCameraNode()->yaw(Ogre::Degree(-YAW_SPEED * me.state.X.rel), Ogre::Node::TS_WORLD);
	mPlayerCamera->getCameraNode()->pitch(Ogre::Degree(-YAW_SPEED * me.state.Y.rel), Ogre::Node::TS_LOCAL);

	mTurret->getSceneNode()->yaw(Ogre::Degree(-YAW_SPEED * me.state.X.rel), Ogre::Node::TS_LOCAL);
	mGun->getSceneNode()->pitch(Ogre::Degree(-PITCH_SPEED * me.state.Y.rel), Ogre::Node::TS_LOCAL);
}
//---------------------------------------------------------------------------
void PlayerTank::shootMissle()
{
	if (mMisslesNum > 0)
	{
		auto m = new Missle(mSceneMgr, mGun.get(), this->getPhysicsEngine());

		mShotMissles.push_back(m);
		mMisslesNum -= 1;
		mWorld->getSoundEngine()->play2D("Content/Sound/missile_launch.flac", false);
	}
}
//---------------------------------------------------------------------------
void PlayerTank::mousePressed(const OIS::MouseEvent& ke, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		shootShell();
	}
	else if (id == OIS::MB_Right)
	{
		shootMissle();
	}
}
//---------------------------------------------------------------------------
void PlayerTank::mouseReleased(const OIS::MouseEvent& ke, OIS::MouseButtonID id)
{
}
//---------------------------------------------------------------------------
void PlayerTank::update(float time)
{
	if (dead) 
		return;
	auto worldOri = mSceneNode->_getDerivedOrientation();
	auto velocity = worldOri * Ogre::Vector3(TANK_INITIAL_SPEED, 0, 0) * mDirection.x;
	this->physicsEngineEntity->setLinearVelocity(btVector3(velocity.x, 0, velocity.z));
	this->physicsEngineEntity->setAngularVelocity(btVector3(0, mRotation, 0));
	mTimeElapsed += time;

	auto worldPos = mSceneNode->_getDerivedPosition();
	mWorld->getMiniMapMgr()->updatePlayer(&worldPos, &worldOri);

	if (mLife <= 0) {
		dead = true;
		explodedForShell();
	}
}
//---------------------------------------------------------------------------
void PlayerTank::attachCamera(Ogre::Camera *camera, Ogre::Vector3 *pos)
{
	mPlayerCamera = std::unique_ptr<TankCamera>(new TankCamera(camera, this, pos));
	//mPlayerCamera->getCamera()->lookAt(Ogre::Vector3(0,-10,0));
}
//---------------------------------------------------------------------------
void PlayerTank::equipSecondary(int num)
{
	mMisslesNum += num;
}
//---------------------------------------------------------------------------
int PlayerTank::getSecondaryWeaponNum() const
{
	return mMisslesNum;
}
void PlayerTank::createMine(){
	Mine *mine = new Mine(mSceneMgr, this, physicsEngine);
}
