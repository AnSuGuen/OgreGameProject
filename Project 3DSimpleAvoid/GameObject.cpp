#include "GameObject.h"

GameObject::GameObject(void)
{

}

GameObject::~GameObject(void)
{

}

void GameObject::connectScenegraph(Ogre::SceneManager* SceneMgr)
{
	mObjectNode = SceneMgr->getSceneNode("GameObjectRoot")->createChildSceneNode(ObjectNodeName);
	mObjectNode->setPosition(mPosition);
	mObjectYaw = mObjectNode->createChildSceneNode(YawNodeName);

	mObjectEntity = SceneMgr->createEntity(EntityName, EntityFileName);
	mObjectYaw->attachObject(mObjectEntity);
	mObjectEntity->setCastShadows(true);
}