#include "enemy.h"

Enemy::Enemy() : Entity()
{
	spriteData.width = enemyNS::WIDTH;
	spriteData.height = enemyNS::HEIGHT;
	spriteData.x = enemyNS::X;
	spriteData.y = enemyNS::Y;
	spriteData.rect.bottom = enemyNS::HEIGHT;
	spriteData.rect.right = enemyNS::WIDTH;
	velocity.x = 0;
	velocity.y = 0;
	frameDelay = .3f;
	startFrame = 15;
	endFrame = 17;
	currentFrame = startFrame;
	edge.top = -enemyNS::HEIGHT / 2;             // set collision edges
	edge.bottom = enemyNS::HEIGHT / 2;
	edge.left = -enemyNS::WIDTH / 2;
	edge.right = enemyNS::WIDTH / 2;
	collisionType = entityNS::BOX;
	mass = enemyNS::MASS;
}

void Enemy::update(float frameTime)
{
	Entity::update(frameTime);
	spriteData.x += frameTime * velocity.x;
	spriteData.y += frameTime * velocity.y;

	// printf("%f\n", velocity.y);
	// IF COLLIDES WITH A PIECE OF TERRAIN
}