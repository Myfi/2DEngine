#include "player.h"

Player::Player() : Entity()
{
	spriteData.width = playerNS::WIDTH;
	spriteData.height = playerNS::HEIGHT;
	spriteData.x = playerNS::X;
	spriteData.y = playerNS::Y;
	spriteData.rect.bottom = playerNS::HEIGHT;
	spriteData.rect.right = playerNS::WIDTH;
	velocity.x = 0;
	velocity.y = playerNS::SPEED;
	frameDelay = .1f;
	startFrame = 0;
	endFrame = 0;
	currentFrame = startFrame;
	edge.top = -playerNS::HEIGHT / 2;             // set collision edges
	edge.bottom = playerNS::HEIGHT / 2;
	edge.left = -playerNS::WIDTH / 5;
	edge.right = playerNS::WIDTH / 5;
	collisionType = entityNS::BOX;
	mass = playerNS::MASS;
	canJump = false;
}

void Player::update(float frameTime)
{
	VECTOR2 cv;
	if (input->isKeyDown(CHARACTER_RIGHT_KEY))            // if move right
	{
		setVelocity(VECTOR2(100, getVelocity().y));
	}
	if (input->isKeyDown(CHARACTER_LEFT_KEY))             // if move left
	{
		setVelocity(VECTOR2(-100, getVelocity().y));
	}

	if (getX() > GAME_WIDTH)               // if off screen right
		setX((float)- getWidth());     // position off screen left
	if (getX() < - getWidth())         // if off screen left
		setX((float)GAME_WIDTH);           // position off screen right

	// If up key is pressed and the player is at the bottom of the screen
	if (input->isKeyDown(CHARACTER_UP_KEY) && getJump())
	{
		jump();
		setJump(false);
	}
	if (input->isKeyDown(CHARACTER_DOWN_KEY))             // if move down
	{
		setVelocity(VECTOR2(0, getVelocity().y));
		//if (character.getY() > GAME_HEIGHT)              // if off screen bottom
		//character.setY((float)-character.getHeight());    // position off screen top
	}

	Entity::update(frameTime);
	spriteData.x += frameTime * velocity.x;
	spriteData.y += frameTime * velocity.y;

	// If the character hits the ground
	if (spriteData.y > GAME_HEIGHT - playerNS::HEIGHT && velocity.y > 0)
	{
		spriteData.y = GAME_HEIGHT - playerNS::HEIGHT;
		velocity.y = 0;
		velocity.x = 0;
		canJump = true;
	}

	// printf("%f\n", velocity.y);
	// IF COLLIDES WITH A PIECE OF TERRAIN

	velocity.y += frameTime * GRAVITY;
}

void Player::setJump(bool v)
{
	canJump = v;
}

bool Player::getJump()
{
	return canJump;
}