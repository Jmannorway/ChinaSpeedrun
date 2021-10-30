#include "PhysicsComponent.h"
#include "imgui.h"

#include <b2/b2_world.h>
#include <b2/b2_fixture.h>
#include <b2/b2_circle_shape.h>

#include "PhysicsLocator.h"
#include "PhysicsSystem.h"
#include "Debug.h"

void cs::PhysicsDelta::Step(const Vector2& newPosition, float newAngle)
{
	positionDifference = newPosition - positionPrevious;
	angleDifference = newAngle - anglePrevious;
	positionPrevious = newPosition;
	anglePrevious = newAngle;
}

void cs::PhysicsDelta::Teleport(const Vector2& newPosition, float newAngle)
{
	positionDifference = { 0.f, 0.f };
	angleDifference = 0.f;
	positionPrevious = newPosition;
	anglePrevious = newAngle;
}

cs::PhysicsDelta::PhysicsDelta() : positionDifference({0.f}), angleDifference(0.f), positionPrevious({0.f}), anglePrevious(0.f)
{
}

cs::PhysicsComponent::PhysicsComponent() : body(nullptr), shape(new b2CircleShape)
{
	shape->m_radius = 1.f;
	shape->m_p.SetZero();
	definition.awake = false;
	definition.type = b2_dynamicBody;
}

cs::PhysicsComponent::~PhysicsComponent()
{
	if (body)
	{
		auto _ps(PhysicsLocator::GetPhysicsSystem());
		_ps->world->DestroyBody(body);
	}
}

void cs::PhysicsComponent::ImGuiDrawComponent()
{
	/*
	 * Detecting changes in definition etc. should be automatic
	 */
	bool _recreate(false), _update(false);

	if (ImGui::TreeNodeEx("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const float dragSpeed(0.1f);

		const char* _options = { "Static\0Kinematic\0Dynamic" };
		_recreate |= ImGui::Combo("Type", (int*)&definition.type, _options);

		if (body)
		{
			std::string _positionString = std::to_string(body->GetPosition().x) + ", " + std::to_string(body->GetPosition().y);
			ImGui::Text(&_positionString[0]);
		}

		_update |= ImGui::DragFloat("Gravity Scale", &definition.gravityScale, dragSpeed, -10.0f, 10.0f);
		_update |= ImGui::DragFloat2("Linear Velocity", &definition.linearVelocity.x, dragSpeed * 10.f, -100.f, 100.f);
		_update |= ImGui::DragFloat("Angular Velocity", &definition.angularVelocity, dragSpeed * 10.f, -100.f, 100.f);
		_update |= ImGui::Checkbox("Awake", &definition.awake);

		if (_recreate || ImGui::Button("Recreate Body"))
			QueueForCreation();
		else if (_update)
			QueueForUpdate();

		ImGui::TreePop();
	}
}

void cs::PhysicsComponent::QueueForUpdate()
{
	PhysicsLocator::GetPhysicsSystem()->QueueComponentUpdate(this);
}

void cs::PhysicsComponent::QueueForCreation()
{
	PhysicsLocator::GetPhysicsSystem()->QueueComponentCreate(this);
}
