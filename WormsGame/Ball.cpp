#include "Ball.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"

Ball::Ball(fPoint position, float mass, float velocity, int radius, float angle) : PhysicEntity(position, mass, velocity)
{
	this->angle = angle;
	this->surface = 4 * 3.1415 * pow(PIXEL_TO_METERS(radius), 2);
	this->volume = 4 / 3 * 3.1415 * pow(PIXEL_TO_METERS(radius), 3);
	this->radius = PIXEL_TO_METERS(radius);

	etype = BALL;
	inrest = true;
}

void Ball::Draw()
{
	area.x = position.x;
	area.y = position.y;
	if (App->scene_intro->turn) {
		texture = App->textures->Load("Assets/Potion.png");
		App->renderer->Blit(texture, position.x, position.y);
		area.w = 24;
		area.h = 60;
		if (App->physics->debug) {
			SDL_Rect hitbox{ position.x,position.y,20,46 };
			App->renderer->DrawQuad(hitbox, 0, 255, 255);
		}
	}
	else {
		texture = App->textures->Load("Assets/Spit.png");
		App->renderer->Blit(texture, position.x, position.y);
		area.w = 40;
		area.h = 24;
		if (App->physics->debug) {
			SDL_Rect hitbox{ position.x,position.y,40,24 };
			App->renderer->DrawQuad(hitbox, 0, 255, 255);
		}
	}
}

void Ball::Move()
{
	//position.x++;
}

