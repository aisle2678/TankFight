#include "MiniMapManager.h"
#include "AITank.h"


int ZoomRatio = 1;
int CameraDistance = 30;
int Scale = 20;

MiniMapManager::MiniMapManager(Ogre::Root *root, Ogre::RenderWindow *window): mRoot(root), mWindow(window)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "MiniMapSceneMgr");
	mCamera = mSceneMgr->createCamera("miniMapCamera");

	Ogre::Viewport *vp = mWindow->addViewport(mCamera, 1, 0, 0.8, 0.2, 0.2);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCamera->setAutoAspectRatio((Ogre::Real(vp->getActualWidth())) / (Ogre::Real(vp->getActualHeight())));
	
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));
}

MiniMapManager::~MiniMapManager()
{
}

Ogre::SceneNode* MiniMapManager::addMiniMapEntity(Ogre::Vector3 *pos, const std::string &meshName)
{
	Ogre::Entity *entity = mSceneMgr->createEntity(meshName);
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	Ogre::Vector3 convertedPos = Ogre::Vector3(pos->z/ZoomRatio, -pos->x/ZoomRatio, 0);
	node->setPosition(convertedPos);
	node->attachObject(entity);
	node->scale(Scale, Scale, Scale);
	return node;
}

void MiniMapManager::addPlayerTank(Ogre::Vector3* pos)
{
	Ogre::Vector3 convertedPos = Ogre::Vector3(pos->z/ZoomRatio, -pos->x/ZoomRatio, 0);
	Ogre::Entity *entity = mSceneMgr->createEntity("MiniMapPlayerEntity.mesh");
	player = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	player->attachObject(entity);
	player->setPosition(convertedPos);
	player->scale(Scale, Scale, Scale);

	cameraNode = player->createChildSceneNode();
	cameraNode->attachObject(mCamera);

	cameraNode->setPosition(convertedPos + Ogre::Vector3(0, -1, -CameraDistance));
	mCamera->lookAt(convertedPos);
}

void MiniMapManager::updatePlayer(Ogre::Vector3* pos, Ogre::Quaternion* ori)
{
	Ogre::Vector3 convertedPos = Ogre::Vector3(pos->z/ZoomRatio, -pos->x/ZoomRatio, 0);
	player->setPosition(convertedPos);
	player->setOrientation(ori->w, ori->x, ori->z, -ori->y);
}

void MiniMapManager::updateAI(Ogre::Vector3* pos, Ogre::SceneNode* node)
{
	Ogre::Vector3 convertedPos = Ogre::Vector3(pos->z/ZoomRatio, -pos->x/ZoomRatio, 0);
	node->setPosition(convertedPos);
}

void MiniMapManager::deleteEntity(Ogre::SceneNode* node)
{
	Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();
	while ( itObject.hasMoreElements() )
	{
		Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
		node->getCreator()->destroyMovableObject( pObject );
	}
	node->removeAndDestroyAllChildren();
	mSceneMgr->destroySceneNode(node);
}





