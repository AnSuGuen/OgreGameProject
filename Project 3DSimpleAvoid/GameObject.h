#pragma once

#include "GameState.h"

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);
	void connectScenegraph(Ogre::SceneManager* mSceneMgr);

	Ogre::SceneNode* mObjectNode;
	Ogre::SceneNode* mObjectYaw;
	Ogre::Entity* mObjectEntity;
	Ogre::Vector3 mPosition;
	Ogre::Vector3 mDirection;
	Ogre::Quaternion mQuat;
	Ogre::AnimationState* mAnimationState;

	char* ObjectNodeName;
	char* YawNodeName;
	char* EntityName;
	char* EntityFileName;
};