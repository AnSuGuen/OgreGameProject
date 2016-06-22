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
	mCountTime = 0.0f;
}


Bullet::~Bullet(void)
{

}

void Bullet::animationStateSetting()
{
	mAnimationState = mObjectEntity->getAnimationState("swim");
	mAnimationState->setEnabled(true);
	mAnimationState->setLoop(true);
}

void Bullet::frameStarted(GameManager* game, const Ogre::FrameEvent& evt, Ogre::Vector3 playerPos, int BulletType)
{
	if (mAnimationState->getEnabled())
		mAnimationState->addTime(evt.timeSinceLastFrame);

	mQuat = Ogre::Vector3(Ogre::Vector3::UNIT_Z).getRotationTo(mDirection);
	mObjectYaw->setOrientation(mQuat);
	mObjectYaw->yaw(Ogre::Degree(90));
	mObjectNode->translate(mDirection.normalisedCopy() * mSpeed * evt.timeSinceLastFrame);

	if (1 < BulletType)
	{
		if (mCountTime > 2.0f)
		{
			/* if (mDirection.z < 0) mDirection.z += 1.0f;
			else mDirection.z += -1.0f;

			if (mDirection.x < 0) mDirection.x += 1.0f;
			else mDirection.x += -1.0f;*/

			int randir = rand() % 4 + 1;
			if (1 == randir)	mDirection.z += -1.0f;
			else if(2 == randir) mDirection.x += -1.0f;
			else if (3 == randir) mDirection.z += 1.0f;
			else if (4 == randir) mDirection.x += 1.0f;

			mSpeed += 300.0f;
			mCountTime = 0;
		}
		mCountTime += evt.timeSinceLastFrame;
	}


	if (mObjectNode->getPosition().z > 500.0f){ mObjectNode->setVisible(false); mObjectNode->setPosition(1000, 0, 1000);}
	if (mObjectNode->getPosition().z < -500.0f){ mObjectNode->setVisible(false);  mObjectNode->setPosition(1000, 0, 1000); }
	if (mObjectNode->getPosition().x > 500.0f){ mObjectNode->setVisible(false);  mObjectNode->setPosition(1000, 0, 1000); }
	if (mObjectNode->getPosition().x < -500.0f){ mObjectNode->setVisible(false);  mObjectNode->setPosition(1000, 0, 1000); }
}