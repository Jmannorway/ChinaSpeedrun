#include "PhysicsSystem.h"

#include <b2_world.h>
#include <b2_math.h>

#include "Time.h"

void cs::PhysicsSystem::Update()
{
	world->Step(Time::fixedDeltaTime, velocityIterations, positionIterations);
}

cs::PhysicsSystem::PhysicsSystem(int vIter, int pIter) : world(new b2World(b2Vec2(0.f, -9.81f)))
{

}
