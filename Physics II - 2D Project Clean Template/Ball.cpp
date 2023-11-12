#include "Ball.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

Ball::Ball(fPoint position, float mass, float velocity, float acceleration, int radius, float angle) : PhysicEntity(position, mass, velocity, acceleration)
{
	this->angle = angle;
	this->radius = radius;
	r = 255;
	g = 0;
	b = 0;
	texture = App->textures->Load("../Assets/bala.png");
}

void Ball::Draw()
{
	//App->renderer->DrawCircle(position.x, position.y, radius, (Uint8)r, (Uint8)g, (Uint8)b);
	App->renderer->Blit(texture, position.x - 12, position.y - 11, NULL, 0, 0, 0, 0);
}

void Ball::Move()
{
	//position.x++;
}

void Ball::Recenter() {
	this->position.x = App->scene_intro->Cannon.x + (App->scene_intro->Cannon.w / 2);
	this->position.y = App->scene_intro->Cannon.y + (App->scene_intro->Cannon.h / 2);
}
