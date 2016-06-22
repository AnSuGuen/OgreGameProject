#include "Player.h"

Player::Player(void)
{
	ObjectNodeName = "Player";
	YawNodeName = "PlayerYaw";
	EntityName = "Player";
	EntityFileName = "DustinBody.mesh";

	mPosition = Ogre::Vector3(0, 0, 0);
	mDirection = Ogre::Vector3(0, 0, 0);
	Speed = 300.0f;
	HP = 150.0f;
}

Player::~Player(void)
{
}

void Player::animationStateSetting()
{
	mAnimationState = mObjectEntity->getAnimationState("Idle");
	mAnimationState->setEnabled(true);
	mAnimationState->setLoop(true);
}

void Player::setHp(int value)
{
	HP = value;
}

float Player::getHp()
{
	return HP;
}

void Player::frameStarted(GameManager* game, const Ogre::FrameEvent& evt)
{
	if (mDirection != Ogre::Vector3::ZERO)
	{
		mQuat = Ogre::Vector3(Ogre::Vector3::UNIT_Z).getRotationTo(mDirection);
		mObjectYaw->setOrientation(mQuat);
		mObjectNode->translate(mDirection.normalisedCopy() * Speed * evt.timeSinceLastFrame);

		mAnimationState->setEnabled(false);
		mAnimationState = mObjectEntity->getAnimationState("Run");
		mAnimationState->setEnabled(true);
	}
	else
	{
		mAnimationState->setEnabled(false);
		mAnimationState = mObjectEntity->getAnimationState("Idle");
		mAnimationState->setEnabled(true);
	}

	if (mObjectNode->getPosition().z > 500.0f)
		mObjectNode->translate(0, 0, Speed * evt.timeSinceLastFrame * (-1));
	if (mObjectNode->getPosition().z < -500.0f)
		mObjectNode->translate(0, 0, Speed * evt.timeSinceLastFrame);
	if (mObjectNode->getPosition().x > 500.0f)
		mObjectNode->translate(Speed * evt.timeSinceLastFrame * (-1), 0, 0);
	if (mObjectNode->getPosition().x < -500.0f)
		mObjectNode->translate(Speed * evt.timeSinceLastFrame, 0, 0);
	
	if (mAnimationState->getEnabled())
		mAnimationState->addTime(evt.timeSinceLastFrame);
}

void Player::keyPressed(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_W: mDirection.z += -1.0f; break;
	case OIS::KC_S: mDirection.z += +1.0f; break;
	case OIS::KC_A: mDirection.x += -1.0f; break;
	case OIS::KC_D: mDirection.x += +1.0f; break;
	}
}

void Player::keyReleased(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_W: mDirection.z -= -1.0f; break;
	case OIS::KC_S: mDirection.z -= +1.0f; break;
	case OIS::KC_A: mDirection.x -= -1.0f; break;
	case OIS::KC_D: mDirection.x -= +1.0f; break;
	}
}