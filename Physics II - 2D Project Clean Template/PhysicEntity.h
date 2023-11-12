#pragma once
#include "p2Point.h"

#include "SDL_image/include/SDL_image.h"

class PhysicEntity
{
public:
	PhysicEntity(fPoint position, float mass, float velocity, float acceleration);
	virtual void Draw();
	virtual void Move();
	virtual void Recenter();
	float velocity;
	fPoint velocityVec;
	fPoint position;
	float angle;
protected:
	float mass;
	float acceleration;
	SDL_Texture* texture = nullptr;
};

