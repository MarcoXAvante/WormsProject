#pragma once
#include "Module.h"
#include "Globals.h"
#include "Timer.h"

#include "Ball.h"

enum Collisions {
	No,
	Teleport,
	Iteratively,
	Raycast
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	void setBallPointer(PhysicEntity* ball);
	float angle;
	int velocity;
public:
	bool time1 = true;
	bool time2 = false;
	float time;
	bool mode1 = true;
	bool mode2, mode3 = false;
	bool launch, backtrack = false;
	Timer flytime;
	PhysicEntity* ball;
	PhysicEntity* ballg;
	bool debug;
	float velocityx, velocityy;
	Collisions collision = No;
};