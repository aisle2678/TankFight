#pragma once

class AIManager;
class World;
class TankCamera;
class MainListener;
class InputDispatcher;
class Physics;
class BuffManager;

class TankFight :  public Ogre::WindowEventListener
{
public:
	TankFight();

	~TankFight();

	// Do all the application setup
	bool setup(void);

	// Start runn
	void go(void);

protected:

	//  Load all the requrired resources (Mostly reading the file paths
	//  of various resources from the .cfg files)
	void setupResources(void);

	// Invoke the startup window for all of the Ogre settings (resolution, etc)
	bool configure(void);

	// Create all of the required classes and do setup for all non-rendering tasks
	void createScene(void);

	// Free up all memory & resources created in createScene
	void destroyScene(void);

	// Create the rendering camera (separate from the game camera -- the game camera
	//   contains the logic of how the camera should be moved, the rendering camera
	//   is used by the rendering system to render the scene.  So, the game camera 
	//   decides where the camera should be, and then makes calls to the rendering camera
	//   to move the camera
	void createCamera(void);

	void createViewports(void) const;

	// The FrameListener is what receives callbacks from the rendering loop to handle 
	//  game logic
	void createFrameListener(void);

	// ogre stuff
	std::unique_ptr<Ogre::Root> mRoot;
	Ogre::Camera* mCamera1;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcePath;
	Ogre::OverlaySystem *mOverlaySystem;

	// ogre input system
	OIS::InputManager *mInputManager;
	OIS::Mouse *mMouse;
	OIS::Keyboard *mKeyboard;

	// our stuff
	std::unique_ptr<MainListener> mTankFrameListener;
	std::unique_ptr<AIManager> mAIManager;
	std::unique_ptr<World> mWorld;
	std::unique_ptr<InputDispatcher> mInputDispatcher;
	std::unique_ptr<Physics> physicsEngine;
};