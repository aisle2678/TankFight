#include "TankFight.h"
#include "World.h"
#include "AIManager.h"
#include "MainListener.h"
#include "InputDispatcher.h"
#include "Physics.h"
#include "MiniMapManager.h"

//---------------------------------------------------------------------------
TankFight::TankFight()
	: mCamera1(nullptr), mSceneMgr(nullptr), mWindow(nullptr), mOverlaySystem(nullptr), 
	mInputManager(nullptr), mMouse(nullptr), mKeyboard(nullptr), mInputDispatcher(nullptr)
{
	// Provide a nice cross platform solution for locating the configuration files
	// On windows files are searched for in the current working directory, on OS X however
	// you must provide the full path, the helper function macBundlePath does this for us.
	//  (Note:  This is not fully tested under IOS)
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	mResourcePath = macBundlePath() + "/Contents/Resources/";
#else
	mResourcePath = "";
#endif
}
//---------------------------------------------------------------------------
// Standard destructor
TankFight::~TankFight()
{
}
//---------------------------------------------------------------------------
void TankFight::createCamera()
{
	mCamera1 = mSceneMgr->createCamera("PlayerCam");
}
//---------------------------------------------------------------------------
// We will create a single frame listener, to handle our main event loop.  While we could
// add as many frame listeners as we want (and let the main rendering loop call them),
// we would not have as much control over the order that the listeners are called --
//  so we will just have one listener and handle everything ourselves.
void TankFight::createFrameListener(void)
{
	mTankFrameListener = std::unique_ptr<MainListener>(new MainListener(mWindow, mAIManager.get(), mWorld.get(), mKeyboard, mMouse, physicsEngine.get()));
	mRoot->addFrameListener(mTankFrameListener.get());
	mInputDispatcher->addListener(mTankFrameListener.get());
}
//---------------------------------------------------------------------------
// We will have a single viewport.  If you wanted splitscreen, or a rear-view mirror, then
// you may want multiple viewports.
void TankFight::createViewports(void) const
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera1);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	// Alter the camera aspect ratio to match the viewport
	mCamera1->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------------------------------------------------------
// Here is where we set up all of the non-rendering stuff (our world, various managers, etc)
void TankFight::createScene() 
{
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	// setup input
	mInputManager = OIS::InputManager::createInputSystem( pl );
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true));
	mInputDispatcher = std::unique_ptr<InputDispatcher>(new InputDispatcher(mKeyboard, mMouse));

	//create physics engine
	physicsEngine = std::unique_ptr<Physics>(new Physics());

	// create global objects
	mWorld = std::unique_ptr<World>(new World(mSceneMgr, new MiniMapManager(mRoot.get(), mWindow), mCamera1, mKeyboard, mMouse, mWindow, mInputDispatcher.get(), physicsEngine.get()));
	mAIManager = std::unique_ptr<AIManager>(new AIManager(mWorld.get()));
	mWorld->setAIManager(mAIManager.get());

}
//---------------------------------------------------------------------------
bool TankFight::setup(void)
{
	Ogre::String pluginsPath;
	Ogre::String configPath;
	// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB

#if OGRE_DEBUG_MODE == 1 && (OGRE_PLATFORM != OGRE_PLATFORM_APPLE && OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS)
	// add _d suffix to config files
	pluginsPath = mResourcePath + "plugins_d.cfg";
	configPath = mResourcePath + "ogre_d.cfg";
#else
	pluginsPath = mResourcePath + "plugins.cfg";
	configPath = mResourcePath + "ogre.cfg";

#endif


#endif

	mRoot = std::unique_ptr<Ogre::Root>(
		new Ogre::Root(pluginsPath, 
		configPath, mResourcePath + "Ogre.log"));

	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "TankSMInstance");
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	createScene();

	// When we start the renderer, it will run forever, making callbacks every
	// frame to the frame listeners.  These listeners are where all of the non-
	// rendering work is done.  
	createFrameListener();

	return true;

}
//---------------------------------------------------------------------------
// Configures the application 
//   - returns false if the user chooses to abandon configuration.
bool TankFight::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering 
		// window by passing 'true'
		mWindow = mRoot->initialise(true);
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void TankFight::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;

#if OGRE_DEBUG_MODE == 1 && (OGRE_PLATFORM != OGRE_PLATFORM_APPLE && OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS)
	// add _d suffix to config files
	cf.load(mResourcePath + "resources_d.cfg");
#else
	cf.load(mResourcePath + "resources.cfg");
#endif


	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app,
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				String(macBundlePath() + "/" + archName), typeName, secName);
#else
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
#endif
		}
	}
}
//---------------------------------------------------------------------------
void TankFight::destroyScene()
{
	if(mWindow)
	{
		if(mInputManager)
		{
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = nullptr;
		}
	}
}
//---------------------------------------------------------------------------
void TankFight::go(void)
{
	if (!setup())
		return;

	mRoot->startRendering();
}
