#include "PhysicsComponent.h"
#include "imgui.h"

#include <b2/b2_world.h>
#include <b2/b2_fixture.h>
#include <b2/b2_circle_shape.h>
#include <b2/b2_shape.h>

#include "CollisionShape.h"
#include "PhysicsLocator.h"
#include "PhysicsSystem.h"
#include "Debug.h"

void cs::PhysicsDelta::Move(const Vector2& newPosition, float newAngle)
{
	previousPosition = position;
	previousAngle = angle;
	position = newPosition;
	angle = newAngle;
}

void cs::PhysicsDelta::Teleport(const Vector2& newPosition, float newAngle)
{
	position = newPosition;
	previousPosition = newPosition;
	angle = newAngle;
	previousAngle = newAngle;
}

cs::PhysicsDelta::PhysicsDelta() : position({0.f}), angle(0.f), previousPosition({0.f}), previousAngle(0.f)
{
}

cs::PhysicsComponent::PhysicsComponent() : body(nullptr), shape(nullptr)
{
	type = ComponentMeta::PHYSICS_COMPONENT_TYPE;
	definition.type = b2_staticBody;
}

cs::PhysicsComponent::~PhysicsComponent()
{
	/*
	 * No need to destroy body or shape since body gets destroyed by world
	 * and shape is attached to the body, thus gets destroyed alongside it
	 */
}

void cs::PhysicsComponent::ImGuiDrawComponent()
{
	if (ImGui::TreeNodeEx("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const float dragSpeed(0.1f);

		{
			const char* _options[]{ "Static", "Kinematic", "Dynamic" };
			ImGui::Combo("Type", (int*)&definition.type, _options, IM_ARRAYSIZE(_options));
		}

		if (body)
		{
			std::string _positionString = std::to_string(body->GetPosition().x) + ", " + std::to_string(body->GetPosition().y);
			ImGui::Text(&_positionString[0]);
		}

		const char* _options[]{ "Circle", "Rectangle" };
		int _type(static_cast<int>(shapeDefinition.type));
		if (ImGui::Combo("Shape", &_type, _options, IM_ARRAYSIZE(_options)))
		{
			shapeDefinition.type = static_cast<CollisionShapeDefinition::Type>(_type);
		}

		/*
		 * Maybe a shape should be a component (although that comes with its own implementation horrors
		 * Maybe a shape should be a resource (resources could perhaps also have their own imgui changing/drawing functions)
		 */
		switch (shapeDefinition.type)
		{
			case CollisionShapeDefinition::Type::Circle:
				if (ImGui::TreeNodeEx("Circle Shape", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::DragFloat("Radius", &shapeDefinition.radius, dragSpeed, -1000.f, 1000.f);
					ImGui::TreePop();
				}
				break;

			case CollisionShapeDefinition::Type::Rectangle:
				if (ImGui::TreeNodeEx("Rectangle Shape", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::DragFloat2("Extents", &shapeDefinition.extents.x, dragSpeed, -1000.f, 1000.f);
					ImGui::TreePop();
				}
				
				break;
		}

		ImGui::DragFloat("Gravity Scale", &definition.gravityScale, dragSpeed, -10.0f, 10.0f);
		ImGui::DragFloat2("Linear Velocity", &definition.linearVelocity.x, dragSpeed * 10.f, -100.f, 100.f);
		ImGui::DragFloat("Angular Velocity", &definition.angularVelocity, dragSpeed * 10.f, -100.f, 100.f);
		ImGui::Checkbox("Awake On Start", &definition.awake);

		ImGui::TreePop();
	}
}

void cs::PhysicsComponent::QueueForCreation()
{
	PhysicsLocator::GetPhysicsSystem()->QueueComponentCreate(this);
}

void cs::PhysicsComponent::EnterScene()
{
	QueueForCreation();
}

void cs::PhysicsComponent::ExitScene()
{
	DestroyBody();
}

void cs::PhysicsComponent::AddForce(const Vector2& impulse)
{
	body->ApplyForceToCenter(b2Vec2(impulse.x, impulse.y), true);
}

void cs::PhysicsComponent::UpdateFixtures()
{
	DeleteFixtures();
	body->CreateFixture(shape, 1.f);
}

void cs::PhysicsComponent::DeleteFixtures()
{
	auto _fixtureList = body->GetFixtureList();
	if (_fixtureList)
	{
		body->DestroyFixture(_fixtureList);
	}
}

void cs::PhysicsComponent::UpdateShape()
{
	switch (shapeDefinition.type)
	{
	case CollisionShapeDefinition::Type::Rectangle:
		{
		auto _box = new b2BoxShape();
		_box->SetExtents(b2Vec2(shapeDefinition.extents.x, shapeDefinition.extents.y));
		shape = static_cast<b2Shape*>(_box);
		}
		break;
	case CollisionShapeDefinition::Type::Circle:
		{
		auto _circle = new b2CircleShape();
		_circle->m_radius = shapeDefinition.radius;
		shape = static_cast<b2Shape*>(_circle);
		}
		break;
	default:
		shape = CreateDefaultShape();
	}

	UpdateFixtures();
}

void cs::PhysicsComponent::DestroyBody()
{
	auto _ps(PhysicsLocator::GetPhysicsSystem());
	_ps->world->DestroyBody(body);
	body = nullptr;
}

b2Shape* cs::PhysicsComponent::CreateDefaultShape() const
{
	auto _boxShape(new b2BoxShape);
	_boxShape->SetExtents({ 1.0f, 1.0f });
	return _boxShape;
}

b2FixtureDef cs::PhysicsComponent::CreateDefaultFixtureDefinition() const
{
	b2FixtureDef _fDef;
	_fDef.shape = CreateDefaultShape();
	_fDef.density = 1.f;
	_fDef.isSensor = true;
	return _fDef;
}
