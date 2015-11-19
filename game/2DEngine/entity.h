#ifndef _ENTITY_H
#define _ENTITY_H

#include "image.h"
#include "input.h"
#include "game.h"

namespace entityNS
{
	enum COLLISION_TYPE { NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT };
	const float GRAVITY = 6.67428e-11f;         // gravitational constant
}

class Entity : public Image
{
protected:
	entityNS::COLLISION_TYPE collisionType;
	VECTOR2 center;
	
	RECT edge;
	VECTOR2 corners[4];
	VECTOR2 edge01, edge03;

	float   entA01min, entA01max, entA03min, entA03max;
	float   entB01min, entB01max, entB03min, entB03max;

	VECTOR2 velocity;
	VECTOR2 deltaV;

	float mass;
	float health;
	// float rr;
	float force;
	float gravity;
	Input *input;
	HRESULT hr;
	bool active;
	DWORD pixelsColliding;

	bool initialized;

	virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);

public:
	Entity();
	virtual ~Entity() {}

	virtual const VECTOR2* getCenter()
	{
		center = VECTOR2(getCenterX(), getCenterY());
		return &center;
	}

	virtual const RECT& getEdge() const { return edge; }
	virtual const VECTOR2* getCorner(UINT c) const
	{
		if (c >= 4)
			c = 0;
		return &corners[c];
	}

	// All of the Gets
	virtual const VECTOR2 getVelocity() const { return velocity; }
	virtual bool getActive() const { return active; }
	virtual float getMass() const { return mass; }
	virtual float getGravity() const { return gravity; }
	virtual float getHealth() const { return health; }
	virtual entityNS::COLLISION_TYPE getCollisionType() { return collisionType; }
	virtual float getInitialized() const { return initialized; }

	// All of the Sets
	virtual void setVelocity(VECTOR2 v) { velocity = v; }
	virtual void setDeltaV(VECTOR2 dv) { deltaV = dv; }
	virtual void setActive(bool act) { active = act; }
	virtual void setHealth(float h) { health = h; }
	virtual void setMass(float m) { mass = m; }
	virtual void setGravity(float g) { gravity = g; }
	virtual void setEdge(RECT e) { edge = e; }

	// Functions for:
	// Updating, Initializing, Collision
	virtual void update(float frameTime);
	virtual bool initialize(Game *game, int width, int height, int cols, TextureManager *textureM);
	virtual void activate();
	virtual void ai(float frameTime, Entity &ent);
	virtual bool outsidereect(RECT r);
	virtual int collides(Entity &ent, VECTOR2 &collisionVector);
	void gravityForce(float frameTime);
	void jump(/*VECTOR2 &collisionVector, Entity &ent*/);
};

#endif // !_ENTITY_H
