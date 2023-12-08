#include "PhysicEntity.h"

#include "Application.h"
#include "ModuleRender.h"

PhysicEntity::PhysicEntity(fPoint position, float mass, float velocity)
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

void PhysicEntity::setUpVelocity()
{
	velocityVec.x = velocity * cos(angle * 3.1415 / 180);
	velocityVec.y = velocity * sin(angle * 3.1415 / 180) * -1;
}
