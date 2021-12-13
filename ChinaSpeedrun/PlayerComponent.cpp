#include "PlayerComponent.h"
#include "Debug.h"
#include "Components.h"

void cs::PlayerComponent::Init()
{
	if (gameObject->HasComponent<AudioComponent>())
		ac = &gameObject->GetComponent<AudioComponent>();
	else
		Debug::LogWarning("PlayerComponent: Doesn't have audio component");

	if (gameObject->HasComponent<PhysicsComponent>())
		pc = &gameObject->GetComponent<PhysicsComponent>();
	else
		Debug::LogWarning("PlayerComponent: Doesn't have physics component");
}

void cs::PlayerComponent::Step()
{

}

void cs::PlayerComponent::ImGuiDrawComponent()
{

}

cs::PlayerComponent::PlayerComponent() : ac(nullptr), pc(nullptr)
{
}
