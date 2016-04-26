#pragma once

class World;
class Tank;

const std::string PLAYER_CAMERA_NODE_NAME = "PLAYER_CAMERA_NODE";
const Ogre::Real TC_SPEED = .13f;

class TankCamera
{

public:

	TankCamera(Ogre::Camera *renderCamera, Tank *t, Ogre::Vector3 *pos); 

	void Think(float time);

	Ogre::SceneNode *getCameraNode() const;

	Ogre::Camera *getCamera() const;

protected:

	void attachSelfToTank(Ogre::Vector3 *pos);

	Ogre::Camera *mRenderCamera; 
	Ogre::SceneNode *mSceneNode;

	Tank *mTank;


};
