#include "Missle.h"
#include "World.h"
#include "MiniMapManager.h"

class Physics;

const static Ogre::Vector3 SCALE_VEC = Ogre::Vector3(2.5f, 2.5f, 2.5f);
const static Ogre::Degree YAW_SPEED = Ogre::Degree(.5f);

/// This class represents a "buff" entity.
/// Which will be shown in the world for 
/// user/AI to pick up.

template<typename EntityType>
class BuffEntity : public Entity 
{
public:

	BuffEntity(Ogre::SceneManager* sceneMgr, World* w, Entity* parent, Physics* pe)
		: Entity("", sceneMgr, parent, physicsEngine), mValid(true), mWorld(w)
	{
		mEntity = std::unique_ptr<EntityType>(new EntityType(sceneMgr, parent, pe));
		mEntity->getSceneNode()->scale(SCALE_VEC);
		mEntity->getSceneNode()->pitch(Ogre::Degree(30));
		setRandomPosition();

		Ogre::Vector3 pos = mEntity->getSceneNode()->getPosition();
		miniMapSceneNode = w->getMiniMapMgr()->addMiniMapEntity(&pos, mEntity->getEntity()->getMesh()->getName());
		miniMapSceneNode->yaw(Ogre::Degree(90));
		miniMapSceneNode->scale(3, 3, 3);
	}

	virtual ~BuffEntity()
	{
		destroy();
	}

	void update(float time) override
	{
		auto sn = mEntity->getSceneNode();
		sn->yaw(YAW_SPEED, Ogre::Node::TS_PARENT);
	}

	Ogre::Real getHeight() const override
	{
		return 0;
	}

	Ogre::Real getWidth() const override
	{
		return 0;
	}

	Ogre::Real getLength() const override
	{
		return 0;
	}

	Ogre::Entity *getWrappedEntity() const
	{
		return mEntity->getEntity();
	}

	Ogre::SceneNode *getWrappedSceneNode() const
	{
		return mEntity->getSceneNode();
	}

	void destroy()
	{
		auto sn = mEntity->getSceneNode();
		sn->removeAndDestroyAllChildren();
		mEntity->getSceneManager()->destroySceneNode(sn);

		mWorld->getMiniMapMgr()->deleteEntity(miniMapSceneNode);
	}

	void makeInvalid()
	{
		mValid = false;
		mEntity->getSceneNode()->setVisible(false);
		miniMapSceneNode->setVisible(false);
	}

	bool isValid() const
	{
		return mValid;
	}

protected:

	void setRandomPosition()
	{
		auto worldSize = World::WORLD_SIZE;
		auto x = Ogre::Math::RangeRandom(-worldSize.x / 2, worldSize.x / 2);
		auto z = Ogre::Math::RangeRandom(-worldSize.z / 2, worldSize.z / 2);
		mEntity->getSceneNode()->setPosition(Ogre::Vector3(x, 0, z));
	}

	std::unique_ptr<EntityType> mEntity;

	bool mValid;

	Ogre::SceneNode* miniMapSceneNode;

	World* mWorld;
};
