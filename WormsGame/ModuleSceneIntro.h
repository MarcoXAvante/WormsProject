#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"



#define BOUNCER_TIME 200

enum lightTypes
{
	tiny,
	medium,
	big
};

class ModuleSceneIntro;

struct Light
{
	Light() : texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);

	lightTypes type;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void Reset();

public:

	SDL_Texture* graphics;

	SDL_Texture* tex_light_tiny;
	SDL_Texture* tex_light_medium;
	SDL_Texture* tex_light_big;
	
	uint fx_light_tiny;
	uint fx_light_medium;
	uint fx_light_big;

	p2DynArray<Light> lights;

	uint player_lose_fx;
	SDL_Texture * texture;
	SDL_Texture * texScientist;
	SDL_Texture* texZombie;
	Animation idleScientist;
	Animation walkScientist;
	Animation attackScientist;
	Animation* currentAnimation;
	Animation idleZombie;
	Animation walkZombie;
	Animation attackZombie;
	Animation* currentAnimation2;
	SDL_Rect water;
	SDL_Rect scientist;
	SDL_Rect zombie;

	Ball* bola;

	uint potion;
	uint song;
	uint spit;


	int scientistHP = 3;
	int zombieHP = 3;

	float scientistX;
	float zombieX;
	bool turn = true;
	bool hit = false;
	bool flipScientist = false;
	bool flipZombie = false;
};
