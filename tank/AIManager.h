#pragma once

class World;
class AITank;

const static int MAX_AI_NUM = 30;

class AIManager 
{

public:
	// You may want to add parameters to the AI Manager constructor
	explicit AIManager(World *world);
	~AIManager();

	// Go through all of the AIs, and call think on each one
	void Think(float time);

	int getAINum() const;

	void detachTank();

protected:

	static std::string getNewIdentifier();

	void initializeAIs();
	
	void spawnAI();

	void maintainAINum();

	static int gCurrentAICount;

	World *mWorld;
	// Probablay want a list of individual AI classes here ...

	// stores all AIs.
	std::map<std::string,std::unique_ptr<AITank>> mAIs;
};
