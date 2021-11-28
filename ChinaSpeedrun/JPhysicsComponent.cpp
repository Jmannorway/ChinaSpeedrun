#include "JPhysicsComponent.h"

#include "JCollisionShape.h"

void cs::JPhysicsComponent::Init()
{
	// create body
}

void cs::JPhysicsComponent::ImGuiDrawComponent()
{
	// draw component stuff
}

cs::JPhysicsComponent::JPhysicsComponent() :
	shape(nullptr)
{
}

cs::JPhysicsComponent::~JPhysicsComponent()
{
	delete shape;
}
