#include "JPhysicsComponent.h"

#include "Debug.h"
#include "JCollisionShape.h"
#include "JPhysicsSystem.h"

#include "imgui.h"

void cs::JPhysicsComponent::Init()
{

}

void cs::JPhysicsComponent::Enter()
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

		JCollisionShape::Type _collisionShapeType = JCollisionShape::Type::Invalid;


		const char* const _types[] = { "Invalid", "Sphere", "Plane", "Triangle" };
		int _typeNumberBefore = shape ? static_cast<int>(shape->GetType()) : 0;
		int _typeNumber = _typeNumberBefore;
		ImGui::Combo("Shape type", &_typeNumber, _types, IM_ARRAYSIZE(_types));

		if (_typeNumber != _typeNumberBefore)
		{
			delete shape;

			switch (static_cast<JCollisionShape::Type>(_typeNumber))
			{
			case JCollisionShape::Type::Sphere:		shape = new JCollisionSphere; break;
			case JCollisionShape::Type::Plane:		shape = new JCollisionPlane; break;
			case JCollisionShape::Type::Triangle:	shape = new JCollisionTriangle; break;
			default:								shape = nullptr;
			}
		}

		if (shape)
		{
			shape->ImGuiDraw();
			_collisionShapeType = shape->GetType();
		}

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
