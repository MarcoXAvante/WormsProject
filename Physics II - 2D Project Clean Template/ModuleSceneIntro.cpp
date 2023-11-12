#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
	texture = App->textures->Load("../Assets/canon.png");
}

ModuleSceneIntro::~ModuleSceneIntro()
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;


	Cannon.x = 20;
	Cannon.y = 680;
	Cannon.w = 20;
	Cannon.h = 20;


	fPoint hola;
	hola.x = Cannon.x + (Cannon.w/2);
	hola.y = Cannon.y + (Cannon.h / 2);
	float angle = 45.0f;
	int velocity = 10;

	Ball* bola = new Ball(hola, 5, velocity, 5, 5, angle);
	App->physics->setBallPointer(bola);
	bola = nullptr;

	bola = new Ball(hola, 5, 0, 0, 5, 45);
	App->physics->ballg = bola;
	bola = nullptr;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->DrawLine(0, 700, 1024, 700, 255, 0, 0);
	App->renderer->DrawQuad(Cannon, 125, 33, 129);
	App->renderer->Blit(texture, Cannon.x, Cannon.y);
	App->physics->ball->Draw();
	App->physics->ballg->Draw();
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		Cannon.x += 10 * (App->dt/100);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		Cannon.x -= 10 * (App->dt/100);
	}
	if (!App->physics->launch) {
		App->physics->ball->Recenter();
	}


	return UPDATE_CONTINUE;
}
