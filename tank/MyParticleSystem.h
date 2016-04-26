

class MyParticleSystem
{
public:
	MyParticleSystem();
	~MyParticleSystem();

	Ogre::ParticleSystem *getParticleSystem();

protected:
	Ogre::ParticleSystem * mParticleSystem;
};