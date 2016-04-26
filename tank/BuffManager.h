#pragma once

class World;
class Missle;
class Repair;

template<typename EntityType>
class BuffEntity;

const static float REFRESH_INTERVAL = 10.0f;

class BuffManager
{
public:

	BuffManager(World *world);

	~BuffManager();

	void update(float time);
	
protected:

	void removeAllBuffs();

	void spawnRepairs();

	void refreshBuffs();

	void spawnMissles();

	World *mWorld;

	float mAccumulatedTime;

	// Don't want to do template covariance, so one list a type
	std::vector<std::unique_ptr<BuffEntity<Missle>>> mBuffMissles;
	std::vector<std::unique_ptr<BuffEntity<Repair>>> mBuffRepairs;
};