#include "PhysicsListener.h"
#include "Debug.h"

void cs::PhysicsListener::BeginContact(b2Contact* contact)
{
	Debug::LogInfo("Begin overlap");
}

void cs::PhysicsListener::EndContact(b2Contact* contact)
{
	Debug::LogInfo("End overlap");
}
