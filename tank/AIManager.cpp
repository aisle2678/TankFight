#include "AIManager.h"
#include "World.h"
#include "AITank.h"

//---------------------------------------------------------------------------
int AIManager::gCurrentAICount = 0;
//---------------------------------------------------------------------------
AIManager::AIManager(World *world) : mWorld(world)
{
	initializeAIs();
}
//---------------------------------------------------------------------------
AIManager::~AIManager()
{
	// Clean up after yourself ...
}
//---------------------------------------------------------------------------
void AIManager::Think(float time)
{
	for (auto iter = mAIs.begin(); iter != mAIs.end(); ++iter)
	{
		if (iter->second->ShouldDelete()){
			if (!iter->second->alreadyDeleted()){
				gCurrentAICount--;
				iter->second->Explode();
			}
		}else{
			iter->second->think(time);
		}
	}
}
//---------------------------------------------------------------------------
std::string AIManager::getNewIdentifier()
{
	std::stringstream ss;
	ss << "AI_" << (gCurrentAICount++);
	return ss.str();
}
//---------------------------------------------------------------------------
void AIManager::initializeAIs()
{
	auto worldSize = mWorld->getWorldSize();
	auto sceneMgr = mWorld->getSceneManager();
	std::cout << worldSize << std::endl;
	for (auto i = 0; i < MAX_AI_NUM; ++i) {
		auto x = Ogre::Math::RangeRandom(-worldSize.x/4, worldSize.x/4);
		auto z = Ogre::Math::RangeRandom(-worldSize.z/4, worldSize.z/4);
		auto id = getNewIdentifier();
		auto tank = std::unique_ptr<AITank>(new AITank(sceneMgr, mWorld, mWorld->getPhysicsEngine(), &id)); 
		auto pos = Ogre::Vector3(x, -10, z);
		tank->setPosition(pos);
		tank->getSceneNode()->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0,360)));
		tank->getPhysicsEngineEntity()->activate();
		mAIs[id] = std::move(tank);
	}
}
//---------------------------------------------------------------------------
void AIManager::spawnAI()
{
}
//---------------------------------------------------------------------------
void AIManager::maintainAINum()
{
}
//---------------------------------------------------------------------------
int AIManager::getAINum() const
{
	return gCurrentAICount;
}
