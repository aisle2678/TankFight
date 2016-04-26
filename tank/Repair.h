#include "Entity.h"

class Physics;
class World;

static const std::string REPAIR_MESH = "Repair.mesh";

class Repair : public Entity
{
public:

	Repair(Ogre::SceneManager *sceneMgr, Entity *parent, Physics *pe);

	~Repair();

	Ogre::Real getHeight() const override;

	Ogre::Real getWidth() const override;

	Ogre::Real getLength() const override;
};
