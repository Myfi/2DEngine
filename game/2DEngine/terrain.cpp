#include "terrain.h"

Terrain::Terrain()
{
	spriteData.width = terrainNS::WIDTH;
	spriteData.height = terrainNS::HEIGHT;
	spriteData.x = terrainNS::X;
	spriteData.y = terrainNS::Y;
	spriteData.rect.bottom = terrainNS::HEIGHT;
	spriteData.rect.bottom = terrainNS::WIDTH;
	velocity.x = 0;
	velocity.y = 0;
	frameDelay = .2f;
	startFrame = 0;
	endFrame = 4;
	currentFrame = startFrame;
	edge.top = -terrainNS::HEIGHT /2;             // set collision edges
	edge.bottom = terrainNS::HEIGHT / 2;
	edge.left = -terrainNS::WIDTH / 2;
	edge.right = terrainNS::WIDTH / 2;
	collisionType = entityNS::BOX;
}

void Terrain::update(float frameTime)
{
	Entity::update(frameTime);
}
