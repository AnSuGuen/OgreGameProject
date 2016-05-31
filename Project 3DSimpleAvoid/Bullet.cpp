#include "Bullet.h"


Bullet::Bullet(char* objname, char* yawname, char* entityname, char* entitiyfilename, Ogre::Vector3 dir, Ogre::Vector3 pos)
{
	ObjectNodeName = objname;
	YawNodeName = yawname;
	EntityName = entityname;
	EntityFileName = entitiyfilename;

	mDirection = dir;
	mPosition = pos;
	mSpeed = 200.0f;
}


Bullet::~Bullet(void)
{

}

void Bullet::animationStateSetting()
{
	//mAnimationState = mObjectEntity->getAnimationState("Idle2");
	//mAnimationState->setEnabled(true);
	//mAnimationState->setLoop(true);
}

void Bullet::frameStarted(GameManager* game, const Ogre::FrameEvent& evt, Ogre::Vector3 playerPos, int BulletType)
{
	//if (mAnimationState->getEnabled())
	//	mAnimationState->addTime(evt.timeSinceLastFrame);

	mQuat = Ogre::Vector3(Ogre::Vector3::UNIT_Z).getRotationTo(mDirection);
	mObjectYaw->setOrientation(mQuat);
	mObjectYaw->yaw(Ogre::Degree(90));
	mObjectNode->translate(mDirection.normalisedCopy() * mSpeed * evt.timeSinceLastFrame);
}