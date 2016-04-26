#pragma once

#include "Tank.h"

class Physics;
class TankCamera;
class Turret;
class Gun;
class Missle;

static const int PLAYER_INITIAL_LIFE = 100;

class PlayerTank : public Tank
{
public:
	
	PlayerTank(Ogre::SceneManager* sceneMgr, World* w, Physics *physicEngine);

	~PlayerTank();

	void keyPressed(const OIS::KeyEvent &ke) override;

	void keyReleased(const OIS::KeyEvent &ke) override;

	void mouseMoved(const OIS::MouseEvent &ke) override;

	void shootMissle();

	void mousePressed(const OIS::MouseEvent &ke, OIS::MouseButtonID id) override;

	void mouseReleased(const OIS::MouseEvent &ke, OIS::MouseButtonID id) override;

	void update(float time) override;

	void attachCamera(Ogre::Camera *camera, Ogre::Vector3 *pos);

	void equipSecondary(int num);

	int getSecondaryWeaponNum() const;

	void createMine();

	void collisionHandler(const btVector3 *ptA, const btVector3 *ptB, const btVector3 *normalOnB, Entity *thatNode, std::list<Entity*> *list);

protected:
	// each player camera should be owned by a
	// tank entity
	std::unique_ptr<TankCamera> mPlayerCamera;

	std::vector<Missle*> mShotMissles;

	int mMisslesNum;

	bool dead;
};
