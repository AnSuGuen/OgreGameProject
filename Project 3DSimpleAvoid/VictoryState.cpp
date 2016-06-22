#include "VictoryState.h"
#include "PlayState.h"
#include "TitleState.h"

using namespace Ogre;

VictoryState VictoryState::mVictoryState;

void VictoryState::enter(void)
{
  mContinue = true;
  mVictoryOverlay = OverlayManager::getSingleton().getByName("Overlay/Victory");
  mVictoryOverlay->show();
}

void VictoryState::exit(void)
{
  mVictoryOverlay->hide();
}

void VictoryState::pause(void)
{
}

void VictoryState::resume(void)
{
}

bool VictoryState::frameStarted(GameManager* game, const FrameEvent& evt)
{
  return true;
}

bool VictoryState::frameEnded(GameManager* game, const FrameEvent& evt)
{
	return mContinue;
}

bool VictoryState::keyPressed(GameManager* game, const OIS::KeyEvent &e)
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
