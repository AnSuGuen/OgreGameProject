#include "GameOverState.h"
#include "PlayState.h"
#include "TitleState.h"

using namespace Ogre;

GameOverState GameOverState::mGameOverState;

void GameOverState::enter(void)
{
  mContinue = true;
  mGameOverOverlay = OverlayManager::getSingleton().getByName("Overlay/Gameover");
  mGameOverOverlay->show();
}

void GameOverState::exit(void)
{
  mGameOverOverlay->hide();
}

void GameOverState::pause(void)
{
}

void GameOverState::resume(void)
{
}

bool GameOverState::frameStarted(GameManager* game, const FrameEvent& evt)
{
  return true;
}

bool GameOverState::frameEnded(GameManager* game, const FrameEvent& evt)
{
	return mContinue;
}

bool GameOverState::keyPressed(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_RETURN:
		game->changeState(TitleState::getInstance());
		break;
	case OIS::KC_ESCAPE:
		mContinue = false;
		break;
	}
	return true;
}
