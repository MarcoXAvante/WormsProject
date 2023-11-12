#pragma once
#include "PhysicEntity.h"

class Ball : public PhysicEntity
{
public:
	Ball(fPoint position, float mass, float velocity, float acceleration, int radius, float angle);
	//poner en PhysicEntity con texturas
	void Draw();
	void Move();
	void Recenter();
private:
	int radius;
	int r;
	int g;
	int b;
};

