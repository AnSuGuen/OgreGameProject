#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(char* objname, char* yawname, char* entityname, char* entitiyfilename,
		Ogre::Vector3 Direction, Ogre::Vector3 pos);
	~Bullet(void);
	void animationStateSetting();

	void frameStarted(GameManager* game, const Ogre::FrameEvent& et, Ogre::Vector3 playerPos, int BulletType);

private:
	Ogre::AnimationState* mAnimationState_BLOCK;

	float mSpeed;
	float mAccel;
	int mCountTime;
};

