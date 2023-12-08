#pragma once
#include "PhysicEntity.h"

class Ball : public PhysicEntity
{
public:
	Ball(fPoint position, float mass, float velocity, int radius, float angle);
	//poner en PhysicEntity con texturas
	void Draw();
	void Move();
	
	SDL_Rect area;
	bool inrest;
private:
	int radius;
	int r;
	int g;
	int b;
};

