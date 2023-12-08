#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	return true;
}


update_status ModulePhysics::PreUpdate()
{
	time = (App->dt / 1000);
	Calculate_Gravity();
	if (checkCollision(pObjects.at(0), pObjects.at(2))) {
		Calculate_Hydrodinamics();
	}
	else {
		Calculate_Aerodynamics();
	}

	
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		enableLift = !enableLift;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		enableLift = !enableWater;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{
 		switch (integrator)
		{
		case Integrators::EULER:
			integrator = Integrators::SYMPLETIC;
			break;

		case Integrators::SYMPLETIC:
			integrator = Integrators::VERLET;
			break;

		case Integrators::VERLET:
			integrator = Integrators::EULER;
			break;

		default:
			break;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) 
	{
		switch (collision)
		{
		case Collisions::NO:
			collision = Collisions::TELEPORT;
			break;

		case Collisions::TELEPORT:
			collision = Collisions::ITERATIVE;
			break;

		case Collisions::ITERATIVE:
			collision = Collisions::RAYCAST;
			break;

		case Collisions::RAYCAST:
			collision = Collisions::NO;
			break;

		default:
			break;
		}
	}

	switch (integrator)
	{
	case Integrators::EULER:
		Integrator_Euler();
		break;

	case Integrators::SYMPLETIC:
		Integrator_SympleticEuler();
		break;

	case Integrators::VERLET:
		Integrator_VelocityVerlet();
		break;

	default:
		break;
	}

	if (App->scene_intro->bola->active)
	{
		switch (collision)
		{
		case Collisions::NO:
			Collision_NoAdjustment();
			break;

		case Collisions::TELEPORT:
			Collision_Teleport();
			break;

		case Collisions::ITERATIVE:
			Collision_Iterative();
			break;

		case Collisions::RAYCAST:
			Collision_Raycast();
			break;

		default:
			break;
		}
	}

	return UPDATE_CONTINUE;
}


update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;
	
	if (!debug) {
		App->window->SetTitle(TITLE);
		return UPDATE_CONTINUE;
	}


	static char title[256];
	sprintf_s(title, 256, "Initial Vel: %.2f Angle: %.2f VelX: %.2f VelY: %.2f, dt: %.2f, airDen: %.2f, integrator: %d, collision: %d",
		pObjects.at(0)->velocity, pObjects.at(0)->angle, pObjects.at(0)->velocityVec.x, pObjects.at(0)->velocityVec.y, App->dt, airDensity, integrator, collision);

	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}

void ModulePhysics::Calculate_Gravity()
{
	for (size_t i = 0; i < pObjects.size(); i++)
	{
		if (pObjects.at(i)->active == true) 
		{
			pObjects.at(i)->force.y = pObjects.at(i)->mass * App->gravity;
			pObjects.at(i)->force.x = 0;
		}
	}
}

void ModulePhysics::Calculate_Aerodynamics()
{
	for (size_t i = 0; i < pObjects.size(); i++)
	{
		if (pObjects.at(i)->active == true)
		{
			if (enableLift) {
				float lift = (0.5 * (airDensity * (pow(pObjects.at(i)->velocityVec.y, 2)) * pObjects.at(i)->surface * liftCoeficient));
				if (pObjects.at(i)->velocityVec.y > 0.0f) {
					pObjects.at(i)->force.y -= lift;
				}

				float dragy = (0.5 * (airDensity * (pow(pObjects.at(i)->velocityVec.y, 2)) * pObjects.at(i)->surface * dragCoeficient));
				float dragx = (0.5 * (airDensity * (pow(pObjects.at(i)->velocityVec.x, 2)) * pObjects.at(i)->surface * dragCoeficient));
				if (pObjects.at(i)->velocityVec.y < 0.0f)
				{
					pObjects.at(i)->force.y += dragy;
					if (pObjects.at(i)->velocityVec.x > 0.0f)
					{
						pObjects.at(i)->force.x -= dragx;
					}
					else {
						pObjects.at(i)->force.x += dragx;
					}
				}
			}
		}
	}
}

void ModulePhysics::Calculate_Hydrodinamics()
{
	for (size_t i = 0; i < pObjects.size(); i++)
	{
		if (enableWater) {
			//Lift is negligible because it's really small

			// Drag
			float dragx = bCoeficient * pObjects.at(0)->velocityVec.x;
			float dragy = bCoeficient * pObjects.at(0)->velocityVec.y;

			if (pObjects.at(0)->velocityVec.y > 0.0f)
			{
				pObjects.at(i)->force.y += dragy;
			}
			else {
				pObjects.at(i)->force.y -= dragy;
			}

			pObjects.at(i)->force.x -= dragx;

			//Buoyancy
			float buoyancy = (App->gravity * fluidDensity * pObjects.at(i)->volume);
			pObjects.at(i)->force.y -= buoyancy;
		}
	}
}

void ModulePhysics::Integrator_Euler()
{
	for (size_t i = 0; i < pObjects.size(); i++)
	{
		if (pObjects.at(i)->active == true) 
		{
			// Y
			pObjects.at(i)->acceleration.y = (pObjects.at(i)->force.y / pObjects.at(i)->mass);
			pObjects.at(i)->position.y += METERS_TO_PIXELS(pObjects.at(i)->velocityVec.y) * (App->dt / 1000);
			pObjects.at(i)->velocityVec.y += pObjects.at(i)->acceleration.y * (App->dt / 1000);

			// X
			pObjects.at(i)->acceleration.x = (pObjects.at(i)->force.x / pObjects.at(i)->mass);
			pObjects.at(i)->position.x += METERS_TO_PIXELS(pObjects.at(i)->velocityVec.x) * (App->dt / 1000);
			pObjects.at(i)->velocityVec.x += pObjects.at(i)->acceleration.x * (App->dt / 1000);
		}
	}
}

void ModulePhysics::Integrator_SympleticEuler()
{
	for (size_t i = 0; i < pObjects.size(); i++)
	{
		if (pObjects.at(i)->active)
		{
			// Y
			pObjects.at(i)->acceleration.y = (pObjects.at(i)->force.y / pObjects.at(i)->mass);
			pObjects.at(i)->velocityVec.y += pObjects.at(i)->acceleration.y * (App->dt / 1000);
			pObjects.at(i)->position.y += METERS_TO_PIXELS(pObjects.at(i)->velocityVec.y) * (App->dt / 1000);

			// X
			pObjects.at(i)->acceleration.x = (pObjects.at(i)->force.x / pObjects.at(i)->mass);
			pObjects.at(i)->velocityVec.x += pObjects.at(i)->acceleration.x * (App->dt / 1000);
			pObjects.at(i)->position.x += METERS_TO_PIXELS(pObjects.at(i)->velocityVec.x) * (App->dt / 1000);
		}
	}
}

void ModulePhysics::Integrator_VelocityVerlet()
{
	for (size_t i = 0; i < pObjects.size(); i++)
	{
		if (pObjects.at(i)->active)
		{
			// Y
			pObjects.at(i)->acceleration.y = (pObjects.at(i)->force.y / pObjects.at(i)->mass);
			pObjects.at(i)->position.y += METERS_TO_PIXELS(pObjects.at(i)->velocityVec.y) * (App->dt / 1000) + 0.5 * pObjects.at(i)->acceleration.y * pow(App->dt / 1000, 2);
			pObjects.at(i)->velocityVec.y += pObjects.at(i)->acceleration.y * (App->dt / 1000);

			// X
			pObjects.at(i)->acceleration.x = (pObjects.at(i)->force.x / pObjects.at(i)->mass);
			pObjects.at(i)->position.x += METERS_TO_PIXELS(pObjects.at(i)->velocityVec.x) * (App->dt / 1000) + 0.5 * pObjects.at(i)->acceleration.x * pow(App->dt / 1000, 2);
			pObjects.at(i)->velocityVec.x += pObjects.at(i)->acceleration.x * (App->dt / 1000);
		}
	}
}

void ModulePhysics::Bounce(size_t i)
{
	if (pObjects.at(i)->bounceCoef > 0.00)
	{
		pObjects.at(0)->force.y -= ((pObjects.at(0)->velocityVec.y * pObjects.at(i)->bounceCoef) / time) * pObjects.at(0)->mass;
		pObjects.at(0)->velocityVec.x *= pObjects.at(i)->initial_bounceCoef;

		pObjects.at(0)->acceleration.y = (pObjects.at(0)->force.y / pObjects.at(0)->mass);
		pObjects.at(0)->velocityVec.y = pObjects.at(0)->acceleration.y * time;
		pObjects.at(i)->bounceCoef -= 0.15;
	}
	else
	{
		pObjects.at(0)->active = false;
		App->scene_intro->Reset();
	}
	pObjects.at(0)->velocityVec.x *= 0.75;

}

void ModulePhysics::Collision_NoAdjustment()
{
	for (size_t i = 1; i < pObjects.size(); i++)
	{
		if (pObjects.at(0)->position.y + 11 > pObjects.at(i)->position.y && (pObjects.at(i)->etype == GROUND))
		{
			pObjects.at(0)->active = false;
		}
	}
}

void ModulePhysics::Collision_Teleport()
{
	for (size_t i = 1; i < pObjects.size(); i++)
	{
		if (pObjects.at(0)->position.y > pObjects.at(i)->position.y && (pObjects.at(i)->etype == GROUND))
		{
			pObjects.at(0)->position.y = pObjects.at(1)->position.y - (App->scene_intro->bola->width/2);

			Bounce(i);
		}
	}
}

void ModulePhysics::Collision_Iterative()
{
	for (size_t i = 1; i < pObjects.size(); i++)
	{
		if (pObjects.at(0)->position.y > pObjects.at(i)->position.y && (pObjects.at(i)->etype == GROUND))
		{
			while (pObjects.at(0)->position.y > pObjects.at(i)->position.y && pObjects.at(i)->bounceCoef < 0.00)
			{
				pObjects.at(0)->position.x -= pObjects.at(0)->velocityVec.x;
				pObjects.at(0)->position.y -= pObjects.at(0)->velocityVec.y;
			}

			Bounce(i);
		}
	}
}

void ModulePhysics::Collision_Raycast()
{
	for (size_t i = 1; i < pObjects.size(); i++)
	{
		int previousx = 0, previousy = 0;
		if (pObjects.at(0)->position.y > pObjects.at(i)->position.y && (pObjects.at(i)->etype == GROUND))
		{
			Bounce(i);
		}
	}
}

void ModulePhysics::setUpVelocity()
{
	for (size_t i = 0; i < pObjects.size(); i++)
	{
		if (pObjects.at(i)->active == true)
		{
			pObjects.at(i)->setUpVelocity();
		}
	}
}

void ModulePhysics::reset(PhysicEntity* pObjects)
{
	pObjects->force.y = 0;
	pObjects->force.x = 0;
	pObjects->acceleration.y = 0;
	pObjects->velocityVec.y = 0;
	pObjects->acceleration.x = 0;
	pObjects->velocityVec.x = 0;
}

bool ModulePhysics::checkCollision(PhysicEntity* first, PhysicEntity* second) {
	// Check if rectA and rectB overlap
	if (first->position.x > second->position.x && first->position.x < second->position.x + second->width) {
		if (first->position.y > second->position.y && first->position.y < second->position.y + second->height) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}