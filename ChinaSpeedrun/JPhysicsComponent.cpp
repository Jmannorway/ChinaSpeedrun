#include "JPhysicsComponent.h"

#include "JCollisionShape.h"

#include "imgui.h"

void cs::JPhysicsComponent::Init()
{
	// create body
}

void cs::JPhysicsComponent::ImGuiDrawComponent()
{
	if (ImGui::TreeNodeEx("JPhysics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Mass", &mass);
		ImGui::DragFloat3("Velocity", &velocity.x);
		ImGui::DragFloat3("Force", &force.x);

		ImGui::TreePop();
	}
}

cs::JPhysicsComponent::JPhysicsComponent() :
	mass(0.1f), velocity(0.f), force(0.f), shape(nullptr)
{
	type = ComponentMeta::JPHYSICS_COMPONENT_TYPE;
}

cs::JPhysicsComponent::~JPhysicsComponent()
{
	delete shape;
}
