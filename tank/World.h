#pragma once

#include "Physics.h"
#include "irrKlang.h"

class TankCamera;
class InputDispatcher;
class AIManager;
class BuffManager;
class PlayerTank;
class Tank;
class Entity;
class MiniMapManager;

const std::string LIFE_OVERLAY = "Life";
const std::string LIFE_TEXT = "Life/Panel/Text";

const std::string REMAIN_OVERLAY = "Remain";
const std::string REMAIN_TEXT = "Remain/Panel/Text";

const std::string WEAPON_OVERLAY = "Weapon";
const std::string WEAPON_TEXT = "Weapon/Panel/Text";


class World
{
public:
	const static Ogre::Real PADDING;
	const static Ogre::Vector3 WORLD_SIZE;

	World(Ogre::SceneManager *sceneManager, MiniMapManager* miniMgr, Ogre::Camera *c, OIS::Keyboard *kb, OIS::Mouse *ms,Ogre::RenderWindow *mw, InputDispatcher *id, Physics *physicsEngine);

	~World();

	void update(float time);

	Ogre::SceneManager *getSceneManager() const;

	Ogre::Camera *getRenderCamera() const;

	PlayerTank *getPlayerTank() const;

	AIManager *getAIManager() const;

	MiniMapManager *getMiniMapMgr() const;

	void setAIManager(AIManager *manager);

	static Ogre::Vector3 getWorldSize();

	Physics *getPhysicsEngine() const;

	irrklang::ISoundEngine* getSoundEngine();

	bool needEvasion(const Ogre::Vector3 pos);

	InputDispatcher *getInputDispatcher() const;

	Ogre::ParticleSystem *mParticleSystem;

	int id;

protected:
	struct GrassVertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};

	void initializeEntities();
	void initializeTerrain() const;
	void initializeDecoratingHouses();
	void initializeMeadow() const;
	void initializeBuffManager();
	void initializeWall1();
	void initializeWall2();
	void initializeWall3();
	void initializeWall4();
	void initializeWalls();
	void initialSound();
	static void initializeOverlays();
	void createGrassMesh() const;
	static void cleanupContent();
	void updateOverlays() const;

	const static Ogre::Real GRASS_WIDTH;
	const static Ogre::Real GRASS_HEIGHT;
	
	Ogre::SceneManager *mSceneManager;
	MiniMapManager *miniMapManager;
	// world object needs the render camera
	// because the entities in the world may
	// need the render camera to display the
	// scene it can see. And the world might
	// also need it to show a global scene.
	Ogre::Camera *mRenderCamera;
	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;

	// stores all entities in the scene.
	std::vector<std::unique_ptr<Entity>> mEntities;
	// stores all buff entities, buff entity is a special
	// kind of entity that does not have a scene node for 
	// itself but really is a wrapper for another entity
	// to make it into a buffer display.
	InputDispatcher *mInputDispatcher;

	std::unique_ptr<BuffManager> mBuffManager;

	PlayerTank *mPlayerTank;
	Physics *physicsEngine;
	AIManager *mAIManager;
	Ogre::RenderWindow *mWindow;
	irrklang::ISoundEngine* mSoundEngine;
};
