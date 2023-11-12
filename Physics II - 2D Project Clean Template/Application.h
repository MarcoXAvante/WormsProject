#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "PerfTimer.h"

enum Frametype  {
	FIJAO,
	VARIABLE,
	SEMIFIXED
};


class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	PerfTimer frameTime;
	float gravity = 10.0f;
	float dt;
	Frametype framerate = FIJAO;
private:
	void AddModule(Module* mod);
	uint32 maxFrameDuration = 16;
};

// Global var made extern for Application ---
extern Application* App;