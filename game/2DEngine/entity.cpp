#include "entity.h"

//==========================================================
// Constructor
//==========================================================
Entity::Entity() : Image()
{
	edge.left = -1;
	edge.right = 1;
	edge.top = 1;
	edge.bottom = -1;
	mass = 1;
	velocity.x = 0;
	velocity.y = 0;
	deltaV.x = 0;
	deltaV.y = 0;
	active = true;
	initialized = false;
	collisionType = entityNS::BOX;
	gravity = entityNS::GRAVITY;
}

void Entity::ai(float frameTime, Entity &ent)
{}

//==========================================================
// For initialization of the Entity
//==========================================================
bool Entity::initialize(Game *gamePtr, int width, int height, int cols, TextureManager *texture)
{
	input = gamePtr->getInput();
	initialized = true;
	active = true;
	return(Image::initialize(gamePtr->getGraphics(), width, height, cols, texture)); 
}

//==========================================================
// Activates the Entity
//==========================================================
void Entity::activate()
{
	active = true;
}

void Entity::update(float frameTime)
{
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	Image::update(frameTime);
	// rotatedBoxReady = false;
}

//==========================================================
// For deciding what kind of collision to implement 
// ATM: only does box collision; Might do rotated box if necessary
//==========================================================
int Entity::collides(Entity &ent, VECTOR2 &collision)
{
	if (!active || !ent.getActive())
		return false;

	if (collisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
		return collideBox(ent, collision);

	return false;
}


//==========================================================
// Box collision
// Called by collides()
// Returns: Returns the side that it collided with
// Left = 1;
// Right = 2;
// Top = 3;
// Bottom = 4;
//==========================================================
bool Entity::collideBox(Entity &ent, VECTOR2 &collision)
{
	if (!active || !ent.getActive())
		return false;

	if ((getCenterX() + edge.right*getScale() >= ent.getCenterX() + ent.getEdge().left*ent.getScale()) &&
		(getCenterX() + edge.left*getScale() <= ent.getCenterX() + ent.getEdge().right*ent.getScale()) &&
		(getCenterY() + edge.bottom*getScale() >= ent.getCenterY() + ent.getEdge().top*ent.getScale()) &&
		(getCenterY() + edge.top*getScale() <= ent.getCenterY() + ent.getEdge().bottom*ent.getScale()))
	{
		// collision = *ent.getCenter() - *getCenter();
		return 1;
	}
	return 0;
}

//==========================================================
// Checks to see if the entity is outside of the rectangle (i.e. hitbox)
// Returns: true if outside, false if inside
//==========================================================
bool Entity::outsidereect(RECT rect)
{
	if (spriteData.x + spriteData.width*getScale() < rect.left ||
		spriteData.x > rect.right ||
		spriteData.y + spriteData.height*getScale() < rect.top ||
		spriteData.y > rect.bottom)
		return true;
	return false;
}

void Entity::jump(/*VECTOR2 &collisionVector, Entity &ent*/)
{
	// Move entities out of collision along collision vector
	velocity.y = -300;
}

//==========================================================
// Force of gravity on this entity
// Calculated by F = mass * acceleration (gravity)
// Using that information creates an always downward force
//	to the Entity
//==========================================================
void Entity::gravityForce(float frameTime)
{
	if (!active)
		return;

	force = gravity * mass;
	VECTOR2 gravityV(getCenterX(), 0-getCenterY());

	Graphics::Vector2Normalize(&gravityV);

	gravityV *= force *frameTime;
	velocity += gravityV;
}


