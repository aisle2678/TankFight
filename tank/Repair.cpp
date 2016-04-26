#include "Repair.h"
#include "World.h"
#include "MiniMapManager.h"

Repair::Repair(Ogre::SceneManager* sceneMgr, Entity* parent, Physics* pe)
	: Entity(REPAIR_MESH, sceneMgr, parent, pe)
{
}

Repair::~Repair()
{
}

Ogre::Real Repair::getHeight() const
{
	return 0;
}

Ogre::Real Repair::getWidth() const
{
	return 0;
}

Ogre::Real Repair::getLength() const
{
	return 0;
}
