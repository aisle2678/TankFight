#include "btBulletDynamicsCommon.h"

class MyMotionState : public btMotionState
{
protected:
    Ogre::SceneNode* mSceneNode;
    btTransform mInitialPosition;

public:
    MyMotionState(const btTransform &initialPosition, Ogre::SceneNode *node);

    virtual ~MyMotionState();

    void setNode(Ogre::SceneNode *node);

    virtual void getWorldTransform(btTransform &worldTrans) const;

    virtual void setWorldTransform(const btTransform &worldTrans);
};