#include "Water.h"

Water::Water(SDL_Rect area) : PhysicEntity(position, mass, velocity)
{
	this->position.x = area.x;
	this->position.y = area.y;
	this->width = area.w;
	this->height = area.h;
	this->quad = area;
	this->bounceCoef = -0.25;
	this->initial_bounceCoef = -0.25;
	this->active = false;
	etype = WATER;
	r = 255;
	g = 0;
	b = 0;
	texture; //= App->textures->Load("../Assets/bala.png");
}

void Water::Draw()
{
	App->renderer->DrawQuad(quad, 0, 0, 255);
}
