#pragma once
#include "PhysicEntity.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"

class Water : public PhysicEntity
{
public:
	Water(SDL_Rect area);
	//poner en PhysicEntity con texturas
	void Draw();

	SDL_Rect quad;
private:
	int r;
	int g;
	int b;
};

