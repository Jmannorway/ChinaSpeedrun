#include "PlayerComponent.h"
#include "Debug.h"
#include "Components.h"
#include "Input.h"
#include "Time.h"

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

	if (gameObject->HasComponent<TransformComponent>())
		tc = &gameObject->GetComponent<TransformComponent>();
	else
		Debug::LogWarning("PlayerComponent: Doesn't have transform component");

	if (!camera)
		Debug::LogWarning("PlayerComponent: No camera is attached");
}

void cs::PlayerComponent::Step()
{
	if (Input::GetMousePressed(0))
	{
		Vector2 _toMouse = Input::mousePosition - Vector2(tc->position.x, tc->position.y);

		pc->AddForce(_toMouse * sensitivity);
	}
}

void cs::PlayerComponent::ImGuiDrawComponent()
{
	if (ImGui::TreeNodeEx("Player Component"))
	{
		ImGui::DragFloat("Sensitivity", &sensitivity, 0.01f * Time::deltaTime);
	}
}

cs::PlayerComponent::PlayerComponent() : sensitivity(1.f), camera(nullptr), ac(nullptr), pc(nullptr), tc(nullptr)
{
}
