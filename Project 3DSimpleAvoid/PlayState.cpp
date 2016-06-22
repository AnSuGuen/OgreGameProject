#include "PlayState.h"
#include "TitleState.h"

using namespace Ogre;

PlayState PlayState::mPlayState;


void PlayState::enter(void)
{
  mRoot = Root::getSingletonPtr(); 
  mRoot->getAutoCreatedWindow()->resetStatistics();

  mSceneMgr = mRoot->getSceneManager("main");
  mCamera = mSceneMgr->getCamera("main");
  mCamera->setPosition(Ogre::Vector3::ZERO);

  _drawGridPlane();
  _setLights();
  _drawGroundPlane();

  mInformationOverlay = OverlayManager::getSingleton().getByName("Overlay/Information");
  mInformationOverlay->show();

  mCharacterRoot = mSceneMgr->getRootSceneNode()->createChildSceneNode("GameObjectRoot");

  mCameraYaw = mCharacterRoot->createChildSceneNode("CameraYaw", Vector3(0.0f, 0.0f, 0.0f));
  mCameraPitch = mCameraYaw->createChildSceneNode("CameraPitch");
  mCameraHolder = mCameraPitch->createChildSceneNode("CameraHolder", Vector3(0.0f, 800.0f, 1300.0f));
  
  mCameraHolder->attachObject(mCamera);
  mCamera->lookAt(mCameraYaw->getPosition());
  
  player = new Player;
  player->connectScenegraph(mSceneMgr);
  player->animationStateSetting();
  player->mObjectNode->setScale(0.5, 0.5, 0.5);
  
  mStageTime = 15.0f;
  mSpawnTime = 0.0f;
  mSpawnTerm = 0;

  mBulletNum = 0;
  mStageState = 1;

  mCollisionTime = -1.0f;
}

void PlayState::exit(void)
{
	mSceneMgr->clearScene();
	mInformationOverlay->hide();
}

void PlayState::pause(void)
{
}

void PlayState::resume(void)
{
}

bool PlayState::frameStarted(GameManager* game, const FrameEvent& evt)
{
	player->frameStarted(game, evt);

	if (1 == mStageState) mSpawnTerm = 1.5f;
	else if (2 == mStageState) mSpawnTerm = 0.5f;

	if (mSpawnTime > mSpawnTerm)
	{
		for (int i = 0; i < 3; i++)
		{

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

			char objname[20];
			sprintf(objname, "Bullet_%d", mBulletNum);
			char yawname[20];
			sprintf(yawname, "BulletYaw%d", mBulletNum);
			char entityname[20];
			sprintf(entityname, "BulletEntity%d", mBulletNum);

			mBulletNum += 1;

			int randDirection = rand() % 4;
			int randPosition = rand() % 1000 - 500;
			Ogre::Vector3 dir;
			Ogre::Vector3 pos;

			if (LEFT == randDirection){ dir = Ogre::Vector3(1, 0, 0); pos = Ogre::Vector3(-500.0f, 100, randPosition); }
			else if (RIGHT == randDirection){ dir = Ogre::Vector3(-1, 0, 0); pos = Ogre::Vector3(500.0f, 100, randPosition); }
			else if (UP == randDirection){ dir = Ogre::Vector3(0, 0, 1);  pos = Ogre::Vector3(randPosition, 100, -500.0f); }
			else if (DOWN == randDirection){ dir = Ogre::Vector3(0, 0, -1);  pos = Ogre::Vector3(randPosition, 100, 500.0f); }

			bullet.push_back(new Bullet(objname, yawname, entityname, "fish.mesh", dir, pos));
			bullet.back()->connectScenegraph(mSceneMgr);
			bullet.back()->animationStateSetting();
			bullet.back()->mObjectYaw->setScale(10, 10, 10);
			bullet.back()->mObjectYaw->yaw(Degree(90));
		}
		mSpawnTime = 0;
	}

	if (mCollisionTime < 0.0f){
		Ogre::AxisAlignedBox playerBox = player->mObjectYaw->_getWorldAABB();
		for (auto b : bullet)
		{
			Ogre::AxisAlignedBox bulletBox = b->mObjectYaw->_getWorldAABB();
			if (bulletBox.intersects(playerBox)){
				player->setHp(player->getHp()- (rand() % 3 + 1));
				mCollisionTime = 1.0f;
				break;
			}
		}
	}


	for (int i = 0; i < bullet.size(); i++)
	{
		bullet[i]->frameStarted(game, evt, player->mObjectNode->getPosition(), 1);
	}

	mSpawnTime += evt.timeSinceLastFrame;
	mStageTime -= evt.timeSinceLastFrame;
	mCollisionTime -= evt.timeSinceLastFrame;
	if (0 > mStageTime)
	{
		for (auto b : bullet)
		{
			b->mObjectNode->setPosition(1000, 0, 1000);
			b->mObjectNode->setVisible(false);
		}
		bullet.clear();
		player->setHp(100);
		mStageTime = 15.0f;
		mStageState += 1;
	}
	return true;
}

bool PlayState::frameEnded(GameManager* game, const FrameEvent& evt)
{
  static Ogre::DisplayString currFps = L"현재 FPS: "; 
  static Ogre::DisplayString avgFps = L"평균 FPS: ";
  static Ogre::DisplayString bestFps = L"최고 FPS: ";
  static Ogre::DisplayString worstFps = L"최저 FPS: ";
  static Ogre::DisplayString remainTime = L"남은시간: ";
  static Ogre::DisplayString position = L"Pos : ";
  static Ogre::DisplayString stage = L"Stage";
  static Ogre::DisplayString coll = L"HP : ";

  OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("AverageFps");
  OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("CurrFps");
  OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("BestFps");
  OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("WorstFps");
  OverlayElement* guiTime = OverlayManager::getSingleton().getOverlayElement("Time");
  OverlayElement* guiPos = OverlayManager::getSingleton().getOverlayElement("Pos");
  OverlayElement* guiStage = OverlayManager::getSingleton().getOverlayElement("Stage");
  OverlayElement* guiColl = OverlayManager::getSingleton().getOverlayElement("Coll");

  const RenderTarget::FrameStats& stats = mRoot->getAutoCreatedWindow()->getStatistics();

  guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
  guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
  guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS));
  guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS));
  guiTime->setCaption(remainTime + StringConverter::toString(mStageTime));
  guiPos->setCaption(position + StringConverter::toString(player->mObjectNode->getPosition().x)
	  + "  " + StringConverter::toString(player->mObjectNode->getPosition().z));
  guiStage->setCaption(stage + StringConverter::toString(mStageState));

  guiColl->setCaption(coll + StringConverter::toString(player->getHp()));

  return true;
}


bool PlayState::keyReleased(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_W: player->keyReleased(game, e); break;
	case OIS::KC_S: player->keyReleased(game, e); break;
	case OIS::KC_A: player->keyReleased(game, e); break;
	case OIS::KC_D: player->keyReleased(game, e); break;
	}

	return true;
}

bool PlayState::keyPressed(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_W: player->keyPressed(game, e); break;
	case OIS::KC_S: player->keyPressed(game, e); break;
	case OIS::KC_A: player->keyPressed(game, e); break;
	case OIS::KC_D: player->keyPressed(game, e); break;

	case OIS::KC_ESCAPE: 
		game->changeState(TitleState::getInstance());
		break;
	}
	return true;
}

bool PlayState::mousePressed(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool PlayState::mouseReleased(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}


bool PlayState::mouseMoved(GameManager* game, const OIS::MouseEvent &e)
{ 
  //mCameraYaw->yaw(Degree(-e.state.X.rel));
  //mCameraPitch->pitch(Degree(-e.state.Y.rel));
  //mCameraHolder->translate(Ogre::Vector3(0, 0, -e.state.Z.rel * 0.1f));
  return true;
}



void PlayState::_setLights(void)
{
  mSceneMgr->setAmbientLight(ColourValue(0.7f, 0.7f, 0.7f));
  mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

  mLightD = mSceneMgr->createLight("LightD");
  mLightD->setType(Light::LT_DIRECTIONAL);
  mLightD->setDirection( Vector3( 1, -2.0f, -1 ) );
  mLightD->setVisible(true);
}

void PlayState::_drawGroundPlane(void)
{
	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"Ground",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		40000, 40000,
		1, 1,
		true, 1, 1, 1,
		Vector3::NEGATIVE_UNIT_Z
		);

	Entity* SkyboxBack = mSceneMgr->createEntity("SkyboxBack", "Ground");
	SkyboxBack->setMaterialName("water2");
	SceneNode* Backnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SkyboxBack", Vector3(0.0f, -10000.0f, -20000.0f));
	Backnode->attachObject(SkyboxBack);
	Backnode->pitch(Degree(90));
	SkyboxBack->setCastShadows(true);

	Entity* SkyboxFront = mSceneMgr->createEntity("SkyboxFront", "Ground");
	SkyboxFront->setMaterialName("water2");
	SceneNode* Frontnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SkyboxFront", Vector3(0.0f, -10000.0f, 20000.0f));
	Frontnode->attachObject(SkyboxFront);
	Frontnode->pitch(Degree(-90));
	Frontnode->yaw(Degree(180));
	SkyboxFront->setCastShadows(true);

	Entity* Skyboxleft = mSceneMgr->createEntity("Skyboxleft", "Ground");
	Skyboxleft->setMaterialName("water2");
	SceneNode* lefttnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Skyboxleft", Vector3(20000.0f, -10000.0f, 0.0f));
	lefttnode->attachObject(Skyboxleft);
	lefttnode->roll(Degree(90));
	lefttnode->yaw(Degree(-90));
	Skyboxleft->setCastShadows(true);

	Entity* SkyboxRight = mSceneMgr->createEntity("SkyboxRight", "Ground");
	SkyboxRight->setMaterialName("water2");
	SceneNode* rightnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SkyboxRight", Vector3(-20000.0f, -10000.0f, 0.0f));
	rightnode->attachObject(SkyboxRight);
	rightnode->roll(Degree(-90));
	rightnode->yaw(Degree(-270));
	SkyboxRight->setCastShadows(true);

	Entity* SkyboxTop = mSceneMgr->createEntity("SkyboxTop", "Ground");
	SkyboxTop->setMaterialName("water2");
	SceneNode* topnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SkyboxTop", Vector3(0.0f, 10000.0f, 0.0f));
	topnode->attachObject(SkyboxTop);
	topnode->pitch(Degree(-180));
	topnode->yaw(Degree(180));

	Entity* SkyboxBottom = mSceneMgr->createEntity("SkyboxBottom", "Ground");
	SkyboxBottom->setMaterialName("water2");
	SceneNode* Bottomnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SkyboxBottom", Vector3(0.0f, -30000.0f, 0.0f));
	Bottomnode->attachObject(SkyboxBottom);
	Bottomnode->yaw(Degree(180));
}

void PlayState::_drawGridPlane(void)
{
}