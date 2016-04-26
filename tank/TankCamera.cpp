#include "TankCamera.h"
#include "Tank.h"
#include "Turret.h"

//---------------------------------------------------------------------------
TankCamera::TankCamera(Ogre::Camera *rc, Tank *t, Ogre::Vector3 *pos)
	: mRenderCamera(rc), mTank(t)
{
	attachSelfToTank(pos);
}
//---------------------------------------------------------------------------
void TankCamera::Think(float time)
{
	// Any code needed here to move the camera about per frame
	//  (use mRenderCamera to get the actual render camera, of course!)
}
//---------------------------------------------------------------------------
Ogre::SceneNode* TankCamera::getCameraNode() const
{
	return mSceneNode;
}
Ogre::Camera* TankCamera::getCamera() const
{
	return mRenderCamera;
}
//---------------------------------------------------------------------------
void TankCamera::attachSelfToTank(Ogre::Vector3 *pos)
{
	mSceneNode = mTank->getTurret()->getSceneNode()->createChildSceneNode();
	mSceneNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
	// detatch from its original scene node and attach to the new scene node.
	auto parentNode = mRenderCamera->getParentSceneNode();
	if (parentNode)
	{
		parentNode->detachObject(mRenderCamera);
	}
	mSceneNode->attachObject(mRenderCamera);

	mRenderCamera->setNearClipDistance(2);
	mSceneNode->setPosition(*pos);
	mSceneNode->yaw(Ogre::Degree(90));
	mSceneNode->pitch(Ogre::Degree(-15));
}
