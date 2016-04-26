#pragma once

class AITank;

class MiniMapManager
{
public:
	MiniMapManager(Ogre::Root *root, Ogre::RenderWindow *window);
	~MiniMapManager();

	Ogre::SceneNode* addMiniMapEntity(Ogre::Vector3 *pos, const std::string &meshName);

	void deleteEntity(Ogre::SceneNode *node);

	void addPlayerTank(Ogre::Vector3 *pos);

	void updatePlayer(Ogre::Vector3 *pos, Ogre::Quaternion* ori);

	void updateAI(Ogre::Vector3 *pos, Ogre::SceneNode *node);

protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode *player;
	Ogre::Root* mRoot;
	Ogre::Camera* mCamera;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneNode *cameraNode;
};
