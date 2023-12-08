#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
	idleScientist.PushBack({ 67,358,109,206 });
	idleScientist.PushBack({ 183,359,109,206 });
	idleScientist.speed = 0.05f;
	walkScientist.PushBack({ 782,97,66, 210 });
	walkScientist.PushBack({ 673,97,88, 210 });
	walkScientist.PushBack({ 563,97,108, 210 });
	walkScientist.PushBack({ 434,97,121, 210 });
	walkScientist.PushBack({ 367,97,66, 210 });
	walkScientist.PushBack({ 278,97,88, 210 });
	walkScientist.PushBack({ 169,97,108, 210 });
	walkScientist.PushBack({ 48,97,121, 210});
	walkScientist.speed = 0.1f;
	attackScientist.PushBack({48, 614, 109, 206 });
	attackScientist.PushBack({155, 614, 72, 212});
	attackScientist.PushBack({228, 614, 104, 212});
	attackScientist.PushBack({332, 614,102,212});
	attackScientist.PushBack({435, 614,114,212});
	attackScientist.loop = false;
	attackScientist.speed = 0.2f;
	idleZombie.PushBack({ 49,353,72,217 });
	idleZombie.PushBack({ 122,353,72,217 });
	idleZombie.speed = 0.05f;
	walkZombie.PushBack({ 693,84,109, 219 });
	walkZombie.PushBack({ 574,84,118, 219 });
	walkZombie.PushBack({ 512,84,61, 219 });
	walkZombie.PushBack({ 430,84,81, 219 });
	walkZombie.PushBack({ 317,84,112, 219 });
	walkZombie.PushBack({ 208,84,108, 219 });
	walkZombie.PushBack({ 148,84,59, 219 });
	walkZombie.PushBack({ 49,84,98, 219 });
	walkZombie.speed = 0.1f;
	attackZombie.PushBack({ 406, 601, 72, 225 });
	attackZombie.PushBack({ 320, 601, 85, 225 });
	attackZombie.PushBack({ 197, 601, 122, 225 });
	attackZombie.PushBack({ 197, 601, 122, 225 });
	attackZombie.PushBack({ 320, 601, 85, 225 });
	attackZombie.PushBack({ 122, 601, 74, 225 });
	attackZombie.loop = false;
	attackZombie.speed = 0.15f;
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
	texture = App->textures->Load("Assets/Background.png");
	winScientist = App->textures->Load("Assets/win-scientist.png");
	winZombie = App->textures->Load("Assets/win-zombie.png");
	texScientist = App->textures->Load("Assets/Scientist.png");
	texZombie = App->textures->Load("Assets/Zombie.png");

	scientistX = 120;
	scientist.x = scientistX;
	scientist.y = 986-209;
	scientist.w = 109;
	scientist.h = 206;

	zombieX = 1700;
	zombie.x = zombieX;
	zombie.y = 986 - 209;
	zombie.w = 109;
	zombie.h = 206;

	fPoint initial_pos;
	initial_pos.x = 536.0f - 435.0f;
	initial_pos.y = 641.0f;

	bola = new Ball(initial_pos, 10, 10, 20, 70);

	potion = App->audio->LoadFx("Assets/glass.wav");
	spit = App->audio->LoadFx("Assets/spit.wav");
	song = App->audio->PlayMusic("Assets/song.wav");

	App->physics->pObjects.push_back(bola);
	App->physics->setUpVelocity();
	bola->active = false;

	SDL_Rect ground;
	ground.x = 100;
	ground.y = 986;
	ground.w = 1820;
	ground.h = 94;
	App->physics->pObjects.push_back(new Ground(ground));


	water = { 1180,72,155,1008 };
	App->physics->pObjects.push_back(new Water(water));

	currentAnimation = &idleScientist;
	currentAnimation2 = &idleZombie;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::Reset() {
	if (turn) {
		App->audio->PlayFx(potion);
	}
	hit = false;
	turn = !turn;
	bola->active = false;
	bola->inrest = true;
	App->physics->reset(bola);
	attackScientist.Reset();
	attackZombie.Reset();
	currentAnimation = &idleScientist;
	currentAnimation2 = &idleZombie;
	for (size_t i = 1; i < App->physics->pObjects.size(); i++)
		App->physics->pObjects.at(i)->bounceCoef = App->physics->pObjects.at(i)->initial_bounceCoef;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	Uint8 r = 255, g = 0, b = 0;
	currentAnimation->Update();
	currentAnimation2->Update();
	App->renderer->Blit(texture, 0, 0);
	bool R = false;
	bool D = false;

	if (bola->inrest) {
		float X;
		float Y;
		if ((App->input->GetMouseX() - bola->position.x) > 0) {
			X = (App->input->GetMouseX() - bola->position.x);
			R = false;
		}
		else {
			X = (bola->position.x - App->input->GetMouseX());
			R = true;
		}

		if ((bola->position.y - App->input->GetMouseY()) > 0) {
			Y = (bola->position.y - App->input->GetMouseY());
			D = false;
		}
		else {
			Y = (App->input->GetMouseY() - bola->position.y);
			D = true;
		}

		if (R == false && D == false) {
			bola->angle = atan((Y) / (X)) * 180 / M_PI;
		}
		else if (R == true && D == false) {
			bola->angle = 180 - atan((Y) / (X)) * 180 / M_PI;
		}
		else if (R == true && D == true) {
			bola->angle = 180 + atan((Y) / (X)) * 180 / M_PI;
		}
		else if (R == false && D == true) {
			bola->angle = 360 - atan((Y) / (X)) * 180 / M_PI;
		}		
		bola->velocity = PIXEL_TO_METERS(sqrt(pow(bola->position.y - App->input->GetMouseY(), 2) + pow(App->input->GetMouseX() - bola->position.x, 2)));
	}



	if (App->physics->debug) {
		App->renderer->DrawLine(bola->position.x, bola->position.y, App->input->GetMouseX(), App->input->GetMouseY(), r, g, b);
		r = 125; g = 33; b = 129;
		App->renderer->DrawQuad(scientist, r, g, b);
		App->renderer->DrawQuad(zombie, r, g, b);
		App->renderer->DrawQuad(water, 0, 0, 255);

		for (size_t i = 0; i < App->physics->pObjects.size(); i++)
		{
			App->physics->pObjects.at(i)->Draw();
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		if (turn) {
			if (scientist.x > 100) {
				scientistX -= 10 * (App->dt / 100);
				scientist.x = scientistX;
				currentAnimation = &walkScientist;
				flipScientist = true;
			}
		}
		else {
			if (zombie.x > 1460) {
				zombieX -= 10 * (App->dt / 100);
				zombie.x = zombieX;
				currentAnimation2 = &walkZombie;
				flipZombie = false;
			}
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnimation = &idleScientist;
		currentAnimation2 = &idleZombie;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		if (turn) {
			if (scientist.x < 1120 - (scientist.w/2)) {
				scientistX += 10 * (App->dt / 100);
				scientist.x = scientistX;
				currentAnimation = &walkScientist;
				flipScientist = false;
			}
		}	
		else {
			if (zombie.x < 1900) {
				zombieX += 10 * (App->dt / 100);
				zombie.x = zombieX;
				currentAnimation2 = &walkZombie;
				flipZombie = true;
			}
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		currentAnimation = &idleScientist;
		currentAnimation2 = &idleZombie;
	}

	if (bola->inrest) 
	{
		if (turn) {
			bola->position.x = scientist.x + 101;
			bola->position.y = scientist.y + 27;
		}
		else {
			bola->position.x = zombie.x + 10;
			bola->position.y = zombie.y + 10;
		}
	}
	else {
		bola->Draw();
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && bola->inrest)
	{
		if (turn) {
			currentAnimation = &attackScientist;
		}
		else {
			App->audio->PlayFx(spit);
			currentAnimation2 = &attackZombie;
		}
		bola->active = true;
		bola->inrest = false;
		bola->setUpVelocity();
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Reset();
	}

	if (turn) {
		if (SDL_HasIntersection(&bola->area, &zombie) && !hit) {
			hit = true;
			zombieHP--;
		}
	}
	else {
		if (SDL_HasIntersection(&bola->area, &scientist) && !hit) {
			hit = true;
			scientistHP--;
		}
	}

	if (scientistHP < 1) {
		App->renderer->Blit(winZombie, (SCREEN_WIDTH / 2) - 300, SCREEN_HEIGHT / 2);
	}
	else if (zombieHP < 1) {
		App->renderer->Blit(winScientist, (SCREEN_WIDTH/2) - 300, SCREEN_HEIGHT/2);
	}

	App->renderer->BlitDX(texScientist, scientist.x, scientist.y, flipScientist, &currentAnimation->GetCurrentFrame());
	App->renderer->BlitDX(texZombie, zombie.x, zombie.y, flipZombie, &currentAnimation2->GetCurrentFrame());


	return UPDATE_CONTINUE;
}
