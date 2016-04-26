#pragma once

#include "InputListener.h"

class InputHandler;
class AIManager;
class World;
class TankCamera;
class Physics;

class MainListener : public Ogre::FrameListener, public InputListener
{
public:

	MainListener::MainListener(Ogre::RenderWindow* rw, AIManager* aiManager, World* world, OIS::Keyboard *kb, OIS::Mouse *ms, Physics *p);

	bool frameStarted(const Ogre::FrameEvent &evt) override;

	void keyPressed(const OIS::KeyEvent &arg) override;

	void physicsEngineTik(Ogre::Real time);

protected:

	void captureInput() const;

	AIManager *mAIManager;
	World *mWorld;

	Ogre::RenderWindow *mRenderWindow;
	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;

	Physics *physicsEngine;

	bool mContinue;
};
