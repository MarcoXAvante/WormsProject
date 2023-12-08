#pragma once
#include "PhysicEntity.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"

class Ground : public PhysicEntity
{
public:
	Ground(SDL_Rect area);
	//poner en PhysicEntity con texturas
	void Draw();

	SDL_Rect quad;
private:
	int r;
	int g;
	int b;
};

