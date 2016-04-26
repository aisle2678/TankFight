#include "MainListener.h"
#include "AIManager.h"
#include "World.h"
#include "Shell.h"
#include "Entity.h"
#include "AITank.h"
#include "PlayerTank.h"


MainListener::MainListener(Ogre::RenderWindow* rw, AIManager* aiManager, World* world, OIS::Keyboard *kb, OIS::Mouse *ms, Physics *p)
	: mAIManager(aiManager), mWorld(world), mRenderWindow(rw), mKeyboard(kb), mMouse(ms), mContinue(true), physicsEngine(p)
{
}

// On every frame, call the appropriate managers
bool MainListener::frameStarted(const Ogre::FrameEvent &evt)
{
	auto time = evt.timeSinceLastFrame;
	if (time > 0.5)
	{
		time = 0.5;
	}

	PlayerTank *playerTank = mWorld->getPlayerTank();
	Ogre::Vector3 playerTankPos = playerTank->getSceneNode()->_getDerivedPosition();
	//miniMapCamera->setPosition(Ogre::Vector3(playerTankPos.x, playerTankPos.y+50, playerTankPos.z));
	physicsEngineTik(time);

	mAIManager->Think(time);

	captureInput();	

	mWorld->update(time);  

	// Ogre will shut down if a listener returns false.  We will shut everything down if
	// either the user presses escape or the main render window is closed.  Feel free to 
	// modify this as you like.
	if (mRenderWindow->isClosed())
	{
		mContinue = false;
	}

	return mContinue;
}

void MainListener::keyPressed(const OIS::KeyEvent& ke)
{
	switch (ke.key)
	{
	case OIS::KC_ESCAPE:
		mContinue = false;
		break;
	default:
		break;
	}
}

void MainListener::captureInput() const
{
	mKeyboard->capture();
	mMouse->capture();
}

void MainListener::physicsEngineTik(Ogre::Real time) {
	if (this->physicsEngine != NULL){
		physicsEngine->getDynamicsWorld()->stepSimulation(time);
 
		int numManifolds = physicsEngine->getDynamicsWorld()->getDispatcher()->getNumManifolds();
		std::list<Entity*> *m_collisionObjects = new std::list<Entity*>;
		for (int i=0;i<numManifolds;i++)
		{
			btPersistentManifold* contactManifold =  physicsEngine->getDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();

			int numContacts = contactManifold->getNumContacts();
			for (int j=0;j<numContacts;j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance()<0.f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;
					Entity *nodeA = static_cast<Entity *>(obA->getUserPointer());
					Entity *nodeB = static_cast<Entity *>(obB->getUserPointer());
					if (nodeA) {
						nodeA->collisionHandler(&ptA, &ptB, &normalOnB, nodeB, m_collisionObjects);
					}
					if (nodeB) {
						nodeB->collisionHandler(&ptB, &ptA, &normalOnB, nodeA, m_collisionObjects);
					}
				}
			}
		}

		m_collisionObjects->sort();
		m_collisionObjects->unique();
		for  (std::list<Entity*>::iterator itr = m_collisionObjects->begin(); itr != m_collisionObjects->end(); ++itr) {
			if ((*itr)->getType() == Entity::ENTITY_AI_TANK) {
				AITank *tank = static_cast<AITank *>(*itr);
				tank->setDeleteFlag();
			}else{
				Ogre::SceneNode *sceneNode = (*itr)->getSceneNode();
				btRigidBody *engineEntity = (*itr)->getPhysicsEngineEntity();
				Ogre::SceneNode::ObjectIterator itObject = sceneNode->getAttachedObjectIterator();
				while ( itObject.hasMoreElements() )
				{
					Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
					sceneNode->getCreator()->destroyMovableObject( pObject );
				}
				sceneNode->removeAndDestroyAllChildren();
				(*itr)->getSceneManager()->destroySceneNode(sceneNode);
				btCollisionObject* obj = (*itr)->getPhysicsEngineEntity();
				btRigidBody* body = btRigidBody::upcast(obj);
				(*itr)->getPhysicsEngine()->deleteRigidBody(body);
				delete (*itr);
			}
		}
		m_collisionObjects->clear();
	}
}
