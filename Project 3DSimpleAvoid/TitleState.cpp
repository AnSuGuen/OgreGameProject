#include "TitleState.h"
#include "PlayState.h"

using namespace Ogre;

TitleState TitleState::mTitleState;

void TitleState::enter(void)
{
  mContinue = true;
  mTitleOverlay = OverlayManager::getSingleton().getByName("Overlay/Title");
  mTitleOverlay->show();
}

void TitleState::exit(void)
{
  mTitleOverlay->hide();
}

void TitleState::pause(void)
{
  std::cout << "TitleState pause\n";
}

void TitleState::resume(void)
{
  std::cout << "TitleState resume\n";
}

bool TitleState::frameStarted(GameManager* game, const FrameEvent& evt)
{
  return true;
}

bool TitleState::frameEnded(GameManager* game, const FrameEvent& evt)
{
	return mContinue;
}

bool TitleState::keyPressed(GameManager* game, const OIS::KeyEvent &e)
{
	switch (e.key)
	{
	case OIS::KC_RETURN:
		game->changeState(PlayState::getInstance());
		break;
	case OIS::KC_ESCAPE:
		mContinue = false;
		break;
	}
	return true;
}
