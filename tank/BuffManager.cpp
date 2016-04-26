#include "BuffManager.h"
#include "BuffEntity.h"
#include "Missle.h"
#include "Repair.h"
#include "World.h"
#include "PlayerTank.h"

//---------------------------------------------------------------------------
BuffManager::BuffManager(World* world)
	: mWorld(world), mAccumulatedTime(0)
{
}
//---------------------------------------------------------------------------
BuffManager::~BuffManager()
{
}
//---------------------------------------------------------------------------
void BuffManager::update(float time)
{
	mAccumulatedTime += time;
	auto player = mWorld->getPlayerTank();
	auto playerPos = player->getSceneNode()->getPosition();
	auto playerAABBOrigin = player->getEntity()->getBoundingBox();
	auto playerMin = playerAABBOrigin.getMinimum() + playerPos;
	auto playerMax = playerAABBOrigin.getMaximum() + playerPos;
	auto playerAABBGlobal = Ogre::AxisAlignedBox(playerMin, playerMax);

	std::for_each(std::begin(mBuffMissles), std::end(mBuffMissles), [&](std::unique_ptr<BuffEntity<Missle>> &be)
	{
		be->update(time);
		// check for AABB intersection
		auto sn = be->getWrappedSceneNode();
		auto bePos = sn->getPosition();
		auto beAABBOrigin = be->getWrappedEntity()->getBoundingBox();
		auto beAABBGlobal = Ogre::AxisAlignedBox(
			beAABBOrigin.getMinimum() + bePos,
			beAABBOrigin.getMaximum() + bePos);

		if (beAABBGlobal.intersects(playerAABBGlobal) && be->isValid())
		{
			player->equipSecondary(3);
			be->makeInvalid();
		}
	});

	std::for_each(std::begin(mBuffRepairs), std::end(mBuffRepairs), [&](std::unique_ptr<BuffEntity<Repair>> &be)
	{
		be->update(time);
		// check for AABB intersection
		auto sn = be->getWrappedSceneNode();
		auto bePos = sn->getPosition();
		auto beAABBOrigin = be->getWrappedEntity()->getBoundingBox();
		auto beAABBGlobal = Ogre::AxisAlignedBox(
			beAABBOrigin.getMinimum() + bePos,
			beAABBOrigin.getMaximum() + bePos);

		if (beAABBGlobal.intersects(playerAABBGlobal) && be->isValid())
		{
			player->increaseLife(3);
			be->makeInvalid();
		}
	});

	refreshBuffs();
}
//---------------------------------------------------------------------------
void BuffManager::removeAllBuffs()
{
	mBuffMissles.clear();
	mBuffRepairs.clear();
}
//---------------------------------------------------------------------------
void BuffManager::spawnRepairs()
{
	for (int i = 0; i < 20; ++i)
	{
		auto sceneManager = mWorld->getSceneManager();
		auto pe = mWorld->getPhysicsEngine();
		auto repair = std::unique_ptr<BuffEntity<Repair>>(
			new BuffEntity<Repair>(sceneManager, mWorld, nullptr, pe));
		mBuffRepairs.push_back(std::move(repair));
	}
}
//---------------------------------------------------------------------------
void BuffManager::refreshBuffs()
{
	if (mAccumulatedTime > REFRESH_INTERVAL)
	{
		removeAllBuffs();
		spawnMissles();
		spawnRepairs();
		// reset timer
		mAccumulatedTime = .0f;
	}
}
//---------------------------------------------------------------------------
void BuffManager::spawnMissles()
{
	for (int i = 0; i < 20; ++i)
	{
		auto sceneManager = mWorld->getSceneManager();
		auto pe = mWorld->getPhysicsEngine();
		auto missle = std::unique_ptr<BuffEntity<Missle>>(
			new BuffEntity<Missle>(sceneManager,mWorld, nullptr, pe));
		mBuffMissles.push_back(std::move(missle));
	}
}
