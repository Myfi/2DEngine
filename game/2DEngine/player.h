#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity.h"
#include "constants.h"
#include "audio.h"

namespace playerNS
{
	const int WIDTH = 32;
	const int HEIGHT = 32;
	// location on screen
	const int X = 0;
	const int Y = 0;
	// pixels per second
	const float SPEED = 100;
	// minumum Y velocity
	const float MIN_VY = 1;
	const float MASS = 1.0f;
	const int STANDING_START_FRAME = 0;
	const int STANDING_END_FRAME = 3;
	const int WALKING_START_FRAME = 4;
	const int WALKING_END_FRAME = 7;
	const int JUMPING_START_FRAME = 8;
	const int JUMPING_END_FRAME = 11;
}

class Player : public Entity
{
	bool canJump;
public:
	Player();
	void update(float frameTime, Audio a);
	void setJump(bool v);
	bool getJump();
};

#endif