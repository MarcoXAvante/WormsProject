#include "PhysicEntity.h"

#include "Application.h"
#include "ModuleRender.h"

PhysicEntity::PhysicEntity(fPoint position, float mass, float velocity, float acceleration)
{
	this->position = position;
	this->mass = mass;
	this->velocity = velocity;
	this->acceleration = acceleration;
}

void PhysicEntity::Draw()
{
	
}

void PhysicEntity::Move()
{
}

void PhysicEntity::Recenter()
{
}
