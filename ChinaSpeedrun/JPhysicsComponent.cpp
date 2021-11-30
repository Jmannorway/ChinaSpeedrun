#include "JPhysicsComponent.h"

#include "JCollisionShape.h"
#include "JPhysicsSystem.h"

#include "imgui.h"

void cs::JPhysicsComponent::Init()
{

}

void cs::JPhysicsComponent::ImGuiDrawComponent()
{
	if (ImGui::TreeNodeEx("JPhysics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Mass", &mass);
		ImGui::DragFloat("Gravity Scale", &gravityScale);
		ImGui::DragFloat3("Velocity", &velocity.x);
		ImGui::DragFloat3("Force", &force.x);

		if (shape)
			shape->ImGuiDrawCollisionShape();

		ImGui::TreePop();
	}
}

cs::JPhysicsComponent::JPhysicsComponent() :
	mass(0.1f), gravityScale(1.f), velocity(0.f), force(0.f), shape(nullptr)
{
	type = ComponentMeta::JPHYSICS_COMPONENT_TYPE;
}

cs::JPhysicsComponent::~JPhysicsComponent()
{
	delete shape;
}
