#pragma once
#include "p2Point.h"

#include "SDL_image/include/SDL_image.h"

struct Force {
	float x;
	float y;
};

enum EntityType
{
	BALL,
	GROUND,
	WATER,
	NONE
};

class PhysicEntity
{
public:
	PhysicEntity(fPoint position, float mass, float velocity);
	virtual void Draw();
	virtual void Move();
	float velocity;
	fPoint velocityVec;
	fPoint position;
	float angle;
	float surface;
	float volume;
	virtual void Recenter();

	void setUpVelocity();

	int width;
	int height;
	Force force;
	bool active = true;
	float mass;
	fPoint acceleration;
	float bounceCoef;
	float initial_bounceCoef;
	SDL_Texture* texture = nullptr;

	EntityType etype;
};

