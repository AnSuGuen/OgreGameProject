#pragma once

#include "GameObject.h"

class Player :public GameObject
{
public:
	Player(void);
	~Player(void);
	void animationStateSetting();
	
	void frameStarted(GameManager* game, const Ogre::FrameEvent& et);

	void keyReleased(GameManager* game, const OIS::KeyEvent &e);
	void keyPressed(GameManager* game, const OIS::KeyEvent &e);

private:
	float Speed;
};

