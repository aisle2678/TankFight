#include "World.h"
#include "Tank.h"
#include "InputDispatcher.h"
#include "Physics.h"
#include "House.h"
#include "AIManager.h"
#include "PlayerTank.h"
#include "BuffManager.h"
#include "irrKlang.h"
#include "Wall.h"

#pragma comment(lib, "irrKlang.lib")

//---------------------------------------------------------------------------
const Ogre::Real World::GRASS_WIDTH = 2;
//---------------------------------------------------------------------------
const Ogre::Real World::GRASS_HEIGHT = 2;
//---------------------------------------------------------------------------
const Ogre::Real World::PADDING = 30;
//---------------------------------------------------------------------------
const Ogre::Vector3 World::WORLD_SIZE = Ogre::Vector3(1000, 0, 1000);
//---------------------------------------------------------------------------
World::World(Ogre::SceneManager *sceneManager, MiniMapManager *miniMgr, Ogre::Camera *c, OIS::Keyboard *kb, OIS::Mouse *ms,Ogre::RenderWindow *mw, InputDispatcher *id, Physics *p)
	: mSceneManager(sceneManager), miniMapManager(miniMgr), mRenderCamera(c), mKeyboard(kb), mMouse(ms), mWindow(mw), mInputDispatcher(id), mPlayerTank(nullptr), physicsEngine(p), id(0)
{
	// Global illumination for now.  Adding individual light sources will make you scene look more realistic
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.8,0.8,0.8));

	physicsEngine->initObjects();

	createGrassMesh();

	initializeEntities();

	initializeTerrain();

	initializeDecoratingHouses();

	initializeBuffManager();

	initializeOverlays();

	initializeWalls();

	//initializeMeadow();

	// Yeah, this should be done automatically for all fonts referenced in an overlay file.
	//  But there is a bug in the OGRE code so we need to do it manually.
	Ogre::ResourceManager::ResourceMapIterator iter = Ogre::FontManager::getSingleton().getResourceIterator();
	while (iter.hasMoreElements()) 
	{ 
		iter.getNext()->load(); 
	}
	mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);
	mSceneManager->setFog(Ogre::FOG_EXP2, fadeColour, 0.002);

	Ogre::Light* directionalLight = mSceneManager->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0, 0));
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

	initialSound();
}
//---------------------------------------------------------------------------
void World::initializeOverlays()
{
	// Show the overlays, we do this before initializing all objects since we need the
	// Console overaly to display debug information.
	auto &om = Ogre::OverlayManager::getSingleton();

	// overlays
	auto lifeOL = om.getByName(LIFE_OVERLAY);
	lifeOL->show();
	auto remainOL = om.getByName(REMAIN_OVERLAY);
	remainOL->show();
	auto weaponOL = om.getByName(WEAPON_OVERLAY);
	weaponOL->show();

	//// Yeah, this should be done automatically for all fonts referenced in an overlay file.
	////  But there is a bug in the OGRE code so we need to do it manually.
	auto iter = Ogre::FontManager::getSingleton().getResourceIterator();
	while (iter.hasMoreElements()) { 
		iter.getNext()->load(); 
	}
}
//---------------------------------------------------------------------------
World::~World()
{
	cleanupContent();
}
//---------------------------------------------------------------------------
void World::update(float time)
{
	std::for_each(std::begin(mEntities), std::end(mEntities), [&](std::unique_ptr<Entity> &e)
	{
		e->update(time);
	});
	mBuffManager->update(time);

	updateOverlays();
}
//---------------------------------------------------------------------------
Ogre::SceneManager* World::getSceneManager() const
{
	return mSceneManager;
}
//---------------------------------------------------------------------------
Ogre::Camera* World::getRenderCamera() const
{
	return mRenderCamera;
}
//---------------------------------------------------------------------------
Ogre::Vector3 World::getWorldSize()
{
	return Ogre::Vector3(WORLD_SIZE.x, 0, WORLD_SIZE.z);
}
//---------------------------------------------------------------------------
PlayerTank* World::getPlayerTank() const
{
	return mPlayerTank;
}
//---------------------------------------------------------------------------
AIManager* World::getAIManager() const
{
	return mAIManager;
}
MiniMapManager* World::getMiniMapMgr() const
{
	return miniMapManager;
}
//---------------------------------------------------------------------------
void World::setAIManager(AIManager *manager)
{
	mAIManager = manager;
}
//---------------------------------------------------------------------------
Physics * World::getPhysicsEngine() const
{
	return physicsEngine;
}
//---------------------------------------------------------------------------
bool World::needEvasion(const Ogre::Vector3 pos)
{
	auto xLimit = Ogre::Math::Abs(WORLD_SIZE.x / 2);
	auto zLimit = Ogre::Math::Abs(WORLD_SIZE.z / 2);

	auto xOffset = Ogre::Math::Abs(pos.x);
	auto zOffset = Ogre::Math::Abs(pos.z);

	if (xLimit - xOffset < PADDING || zLimit - zOffset < PADDING)
	{
		return true;
	}

	for (auto iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		auto entPos = (*iter)->getSceneNode()->getPosition();
		auto dist = pos - entPos;
		if (dist.length() < PADDING) return true;
	}

	return false;
}
//---------------------------------------------------------------------------
InputDispatcher* World::getInputDispatcher() const
{
	return mInputDispatcher;
}

//---------------------------------------------------------------------------
void World::initializeEntities()
{
	auto playerTank = std::unique_ptr<PlayerTank>(new PlayerTank(mSceneManager, this, physicsEngine));
	playerTank->attachCamera(mRenderCamera, &Ogre::Vector3(15, 7, 0));
	mPlayerTank = playerTank.get();

	mInputDispatcher->addListener(playerTank.get());
	mEntities.push_back(std::move(playerTank));
}
//---------------------------------------------------------------------------
void World::initializeTerrain() const
{
	Ogre::Real planeOffset = 0;
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -planeOffset);

	// create a mesh
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		WORLD_SIZE.x, WORLD_SIZE.z, 20, 20,
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_Z);
	auto groundEntity = mSceneManager->createEntity("ground");
	Ogre::SceneNode *groundNode = mSceneManager->getRootSceneNode()->createChildSceneNode("terrain");
	groundNode->attachObject(groundEntity);
	groundEntity->setCastShadows(false);// would be a waste if we let ground cast a shadow
	groundEntity->setMaterialName("Examples/GrassFloor");

	//create the plane entity to the physics engine, and attach it to the node
	Ogre::Real planeThickness = 50.;
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -planeOffset - planeThickness, 0));

	btScalar groundMass(0.); //the mass is 0, because the ground is immovable (static)
	btVector3 localGroundInertia(0, 0, 0);

	btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(WORLD_SIZE.x), btScalar(planeThickness), btScalar(WORLD_SIZE.z)));
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(groundTransform);

	groundShape->calculateLocalInertia(groundMass, localGroundInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
	btRigidBody *groundBody = new btRigidBody(groundRBInfo);
	groundBody->setRollingFriction(0.1);
	groundBody->setFriction(1);

	//add the body to the dynamics world
	this->physicsEngine->addRigidBody(groundBody);
}
//---------------------------------------------------------------------------
void World::initializeDecoratingHouses()
{
	const static Ogre::Real houseY = 11;
	std::vector<Ogre::Vector3> pv;
	pv.push_back(Ogre::Vector3(35, houseY, 15));
	pv.push_back(Ogre::Vector3(63, houseY, 420));
	pv.push_back(Ogre::Vector3(239, houseY, 321));
	pv.push_back(Ogre::Vector3(-234, houseY, -53));
	pv.push_back(Ogre::Vector3(-89, houseY, -253));
	pv.push_back(Ogre::Vector3(-321, houseY, 460));
	pv.push_back(Ogre::Vector3(121, houseY, -62));
	pv.push_back(Ogre::Vector3(480, houseY, -160));
	pv.push_back(Ogre::Vector3(321, houseY, 160));
	pv.push_back(Ogre::Vector3(-121, houseY, -214));
	pv.push_back(Ogre::Vector3(-221, houseY, 365));
	pv.push_back(Ogre::Vector3(-421, houseY, 169));
	pv.push_back(Ogre::Vector3(-468, houseY, 50));
	pv.push_back(Ogre::Vector3(368, houseY, 50));

	std::for_each(begin(pv), end(pv), [=](Ogre::Vector3 pos)
	{
		auto h = std::unique_ptr<House>(new House(mSceneManager, nullptr, physicsEngine, &pos));
		mEntities.push_back(std::move(h));
	});
}
//---------------------------------------------------------------------------
static void fillRegion(Ogre::StaticGeometry *field, Ogre::Entity *entity, 
					   int x1, int x2, int y1, int y2)
{
	// add grass uniformly throughout the field, with some random variations
	for (int x = -x1; x < x2; x += 1)
	{
		for (int z = -y1; z < y2; z += 1)
		{
			Ogre::Vector3 pos(x + Ogre::Math::RangeRandom(-7, 7), -5.5, z + Ogre::Math::RangeRandom(-7, 7));
			Ogre::Quaternion ori(Ogre::Degree(Ogre::Math::RangeRandom(0, 359)), Ogre::Vector3::UNIT_Y);
			Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.85, 1.15), 1);

			field->addEntity(entity, pos, ori, scale);
		}
	}
	field->build();  // build our static geometry (bake the grass into it)
}
//---------------------------------------------------------------------------
void World::initializeMeadow() const
{
	auto grass = mSceneManager->createEntity("Grass", "grass");
	// create a static geometry field, which we will populate with grass
	auto field1 = mSceneManager->createStaticGeometry("Field1");
	field1->setRegionDimensions(Ogre::Vector3(140, 140, 140));
	field1->setOrigin(Ogre::Vector3(-200, 0, 200));
	fillRegion(field1, grass, -70, 70, -100, 40);
	
	auto field2 = mSceneManager->createStaticGeometry("Field2");
	field2->setRegionDimensions(Ogre::Vector3(100, 140, 30));
	field1->setOrigin(Ogre::Vector3(350, 0, -150));
	fillRegion(field2, grass, -20, 80, -80, -50);
	
	auto field3 = mSceneManager->createStaticGeometry("Field3");
	field3->setRegionDimensions(Ogre::Vector3(300, 140, 150));
	field3->setOrigin(Ogre::Vector3(100, 0, 156));
	fillRegion(field3, grass, -20, 280, 15, 125);
	
	auto field4 = mSceneManager->createStaticGeometry("Field4");
	field4->setRegionDimensions(Ogre::Vector3(500, 140, 50));
	field4->setOrigin(Ogre::Vector3(0, 0, 0));
	fillRegion(field4, grass, 0, 500, 35, 85);
}
//---------------------------------------------------------------------------
void World::initializeBuffManager()
{
	mBuffManager = std::unique_ptr<BuffManager>(new BuffManager(this));
}
//---------------------------------------------------------------------------
void World::initializeWall1()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	// create a mesh
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		WORLD_SIZE.x, WORLD_SIZE.z, 20, 20,
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_Z);
	auto groundEntity = mSceneManager->createEntity("ground");
	auto wallNode = mSceneManager->getRootSceneNode()->createChildSceneNode("wall1");
	wallNode->pitch(Ogre::Degree(90));
	std::cout << WORLD_SIZE << std::endl;
	wallNode->translate(Ogre::Vector3(.0f, .0f, -WORLD_SIZE.z / 2));
	wallNode->attachObject(groundEntity);
	groundEntity->setCastShadows(false);// would be a waste if we let ground cast a shadow
	groundEntity->setMaterialName("Examples/Rockwall");
}
//---------------------------------------------------------------------------
void World::initializeWall2()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	// create a mesh
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		WORLD_SIZE.x, WORLD_SIZE.z, 20, 20,
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_Z);
	auto groundEntity1 = mSceneManager->createEntity("ground");
	auto wallNode1 = mSceneManager->getRootSceneNode()->createChildSceneNode("wall2");
	wallNode1->pitch(Ogre::Degree(-90));
	std::cout << WORLD_SIZE << std::endl;
	wallNode1->translate(Ogre::Vector3(.0f, .0f, WORLD_SIZE.z / 2));
	wallNode1->attachObject(groundEntity1);
	groundEntity1->setCastShadows(false);// would be a waste if we let ground cast a shadow
	groundEntity1->setMaterialName("Examples/Rockwall");
}
//---------------------------------------------------------------------------
void World::initializeWall3()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	// create a mesh
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		WORLD_SIZE.x, WORLD_SIZE.z, 20, 20,
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_Z);
	auto groundEntity2 = mSceneManager->createEntity("ground");
	auto wallNode2 = mSceneManager->getRootSceneNode()->createChildSceneNode("wall3");
	wallNode2->roll(Ogre::Degree(-90));
	std::cout << WORLD_SIZE << std::endl;
	wallNode2->translate(Ogre::Vector3(-WORLD_SIZE.x / 2, .0f, .0f));
	wallNode2->attachObject(groundEntity2);
	groundEntity2->setCastShadows(false);// would be a waste if we let ground cast a shadow
	groundEntity2->setMaterialName("Examples/Rockwall");
}
//---------------------------------------------------------------------------
void World::initializeWall4()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	// create a mesh
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		WORLD_SIZE.x, WORLD_SIZE.z, 20, 20,
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_Z);
	auto groundEntity3 = mSceneManager->createEntity("ground");
	auto wallNode3 = mSceneManager->getRootSceneNode()->createChildSceneNode("wall4");
	wallNode3->roll(Ogre::Degree(90));
	std::cout << WORLD_SIZE << std::endl;
	wallNode3->translate(Ogre::Vector3(WORLD_SIZE.x / 2, .0f, .0f));
	wallNode3->attachObject(groundEntity3);
	groundEntity3->setCastShadows(false);// would be a waste if we let ground cast a shadow
	groundEntity3->setMaterialName("Examples/Rockwall");
}
//---------------------------------------------------------------------------
void World::initializeWalls()
{
//	initializeWall1();
//	initializeWall2();
//	initializeWall3();
//	initializeWall4();
	Ogre::Real len = WORLD_SIZE.x * 2;
	Ogre::Real shift = 500;
	new Wall(mSceneManager, physicsEngine, &btVector3(-len-shift, len/2, 0));  //left
	new Wall(mSceneManager, physicsEngine, &btVector3(len+shift, len/2, 0));	//right
	new Wall(mSceneManager, physicsEngine, &btVector3(0, len/2, len+shift));	//front
	new Wall(mSceneManager, physicsEngine, &btVector3(0, len/2, -len-shift));	//back
}
//---------------------------------------------------------------------------
void World::createGrassMesh() const
{
	auto mesh = Ogre::MeshManager::getSingleton().createManual("grass", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	// create a submesh with the grass material
	auto sm = mesh->createSubMesh();
	sm->setMaterialName("Examples/GrassBlades");
	sm->useSharedVertices = false;
	sm->vertexData = OGRE_NEW Ogre::VertexData();
	sm->vertexData->vertexStart = 0;
	sm->vertexData->vertexCount = 12;
	sm->indexData->indexCount = 18;

#if defined(INCLUDE_RTSHADER_SYSTEM)
	MaterialPtr grassMat = MaterialManager::getSingleton().getByName("Examples/GrassBlades");
	grassMat->getTechnique(0)->setSchemeName(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
#endif

	// specify a vertex format declaration for our mesh: 3 floats for position, 3 floats for normal, 2 floats for UV
	auto decl = sm->vertexData->vertexDeclaration;
	decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	decl->addElement(0, sizeof(float) * 3, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	decl->addElement(0, sizeof(float) * 6, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);

	// create a vertex buffer
	auto vb = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer
		(decl->getVertexSize(0), sm->vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	auto verts = static_cast<GrassVertex*>(vb->lock(Ogre::HardwareBuffer::HBL_DISCARD));  // start filling in vertex data

	for (unsigned int i = 0; i < 3; i++)  // each grass mesh consists of 3 planes
	{
		// planes intersect along the Y axis with 60 degrees between them
		auto x = Ogre::Math::Cos(Ogre::Degree(i * 60)) * GRASS_WIDTH / 2;
		auto z = Ogre::Math::Sin(Ogre::Degree(i * 60)) * GRASS_WIDTH / 2;

		for (unsigned int j = 0; j < 4; j++)  // each plane has 4 vertices
		{
			auto& vert = verts[i * 4 + j];

			vert.x = j < 2 ? -x : x;
			vert.y = j % 2 ? 0 : GRASS_HEIGHT;
			vert.z = j < 2 ? -z : z;

			// all normals point straight up
			vert.nx = 0;
			vert.ny = 1;
			vert.nz = 0;

			vert.u = j < 2 ? 0 : 1;
			vert.v = j % 2;
		}
	}

	vb->unlock();  // commit vertex changes

	sm->vertexData->vertexBufferBinding->setBinding(0, vb);  // bind vertex buffer to our submesh

	// create an index buffer
	sm->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer
		(Ogre::HardwareIndexBuffer::IT_16BIT, sm->indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// start filling in index data
	auto indices = static_cast<Ogre::uint16*>(sm->indexData->indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	for (unsigned int i = 0; i < 3; i++)  // each grass mesh consists of 3 planes
	{
		unsigned int off = i * 4;  // each plane consists of 2 triangles

		*indices++ = 0 + off;
		*indices++ = 3 + off;
		*indices++ = 1 + off;

		*indices++ = 0 + off;
		*indices++ = 2 + off;
		*indices++ = 3 + off;
	}

	sm->indexData->indexBuffer->unlock();  // commit index changes
}
//---------------------------------------------------------------------------
void World::cleanupContent()
{
	Ogre::MeshManager::getSingleton().remove("grass");
}
//---------------------------------------------------------------------------
void World::updateOverlays() const
{
	auto & om = Ogre::OverlayManager::getSingleton();

	// check user life
	std::stringstream ss;
	ss << "Life: " << mPlayerTank->getLife();
	//ss << std::setw(3) << mUserScore  << " - " << mAIScore;
	auto te = static_cast<Ogre::TextAreaOverlayElement *>(om.getOverlayElement(LIFE_TEXT));
	te->setCaption(ss.str());

	// check remaining enemies
	ss.str("");
	int num = mAIManager->getAINum();
	ss << "Enemies: " << num;
	te = static_cast<Ogre::TextAreaOverlayElement *>(om.getOverlayElement(REMAIN_TEXT));
	te->setCaption(ss.str());

	// check secondary weapon
	ss.str("");
	ss << "Missles: " << mPlayerTank->getSecondaryWeaponNum();
	te = static_cast<Ogre::TextAreaOverlayElement *>(om.getOverlayElement(WEAPON_TEXT));
	te->setCaption(ss.str());
}

void World::initialSound()
{
	mSoundEngine = irrklang::createIrrKlangDevice();

	if (!mSoundEngine)
	{
		printf("Could not startup engine\n");
	}

	mSoundEngine->play2D("Content/Sound/background_music.flac", true);
}
irrklang::ISoundEngine* World::getSoundEngine()
{
	return mSoundEngine;
}
