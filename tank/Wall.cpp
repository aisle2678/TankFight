#include "Wall.h"
#include "World.h"

Wall::Wall(Ogre::SceneManager* sceneMgr, Physics* physicsEngine, btVector3* origin)
	:Entity(	"", sceneMgr, nullptr, physicsEngine)
{
	mType = ENTITY_WALL;
	Ogre::Real planeThickness = World::WORLD_SIZE.x * 2;
	btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(planeThickness), btScalar(planeThickness), btScalar(planeThickness)));

	btRigidBody *body = PhysicsSetup(groundShape, 0.f, 1.f, origin, nullptr);
}

Wall::~Wall()
{
	
}

void Wall::collisionHandler(const btVector3* ptA, const btVector3* ptB, const btVector3* normalOnB, Entity* thatNode, std::list<Entity*>* list)
{
	if (!thatNode) return;
}

//---------------------------------------------------------------------------
Ogre::Real Wall::getHeight() const
{
	return mEntity->getBoundingBox().getSize().x;
}
//---------------------------------------------------------------------------
Ogre::Real Wall::getWidth() const
{
	return mEntity->getBoundingBox().getSize().z;
}
//---------------------------------------------------------------------------
Ogre::Real Wall::getLength() const
{
	return mEntity->getBoundingBox().getSize().y;
}


