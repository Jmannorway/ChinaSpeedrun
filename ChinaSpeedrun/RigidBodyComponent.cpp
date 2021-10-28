#include "RigidBodyComponent.h"

#include "PhysicsSystem.h"

#include <b2_world.h>
#include <b2_body.h>

cs::RigidBodyComponent::RigidBodyComponent() : body(nullptr)
{
	b2BodyDef def;
	//body = PhysicsSystem::world->CreateBody(&def);
}
