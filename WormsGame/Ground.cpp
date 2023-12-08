#include "Ground.h"

Ground::Ground(SDL_Rect area) : PhysicEntity(position, mass, velocity)
{
	this->position.y = area.y;
	this->width = area.w;
	this->height = area.h;
	this->quad = area;
	this->bounceCoef = 0.75;
	this->initial_bounceCoef = 0.75;
	this->active = false;
	etype = GROUND;
	r = 255;
	g = 0;
	b = 0;
	texture; //= App->textures->Load("../Assets/bala.png");
}

void Ground::Draw()
{
	App->renderer->DrawQuad(quad, 255, 0, 0);
}
