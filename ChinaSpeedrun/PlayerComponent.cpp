#include "PlayerComponent.h"
#include "Debug.h"
#include "Components.h"
#include "Input.h"
#include "Time.h"
#include "ChinaEngine.h"
#include "VulkanEngineRenderer.h"

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

	if (!cc)
		Debug::LogWarning("PlayerComponent: No camera is attached");
}

void cs::PlayerComponent::Step()
{
	{ // Set camera to be aligned with player on x & y axis
		auto& _ctc = cc->gameObject->GetComponent<TransformComponent>();
		auto& _tc = gameObject->GetComponent<TransformComponent>();
		_ctc.position = Vector3(_tc.position.x, _tc.position.y, zoom);
	}

	if (Input::GetMousePressed(0))
	{
		Vector2 _viewportSize;

		{
			int _width, _height;
			ChinaEngine::renderer.GetViewportSize(_width, _height);
			_viewportSize = Vector2(_width, _height);
		}
		
		pc->AddForce((Input::mousePosition - _viewportSize / 2.f) * sensitivity * Vector2(-1.f, 1.f));
	}
}

void cs::PlayerComponent::ImGuiDrawComponent()
{
	if (ImGui::TreeNodeEx("Player Component"))
	{
		ImGui::DragFloat("Sensitivity", &sensitivity, 0.01f * Time::deltaTime);

		ImGui::TreePop();
	}
}

cs::PlayerComponent::PlayerComponent() : sensitivity(1.f), zoom(30.f), cc(nullptr), ac(nullptr), pc(nullptr), tc(nullptr)
{
}
