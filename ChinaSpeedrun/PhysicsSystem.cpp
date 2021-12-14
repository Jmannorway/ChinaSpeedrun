#include "PhysicsSystem.h"

#include <b2/b2_world.h>

#include "Debug.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "Transform.h"
#include "Time.h"

void cs::PhysicsSystem::UpdateWorld()
{
	frequencyCounter += Time::deltaTime;
	while (frequencyCounter >= frequency)
	{
		world->Step(frequency, velocityIterations, positionIterations);
		frequencyCounter -= frequency;
	}
}

void cs::PhysicsSystem::UpdatePositions(PhysicsComponent& pc, TransformComponent& tc)
{
	/*
	 * Body should always exist when it needs to be updated
	 * When the implementation is done this check should not be necessary anymore
	 */
	auto _pos(pc.body->GetPosition());
	auto _ang(pc.body->GetAngle());
	pc.delta.Move({ _pos.x, _pos.y }, _ang);
	Vector2 _positionDifference = pc.delta.GetPositionDifference();
	tc.position.x += _positionDifference.x;
	tc.position.y += _positionDifference.y;
	tc.rotation.x += pc.delta.GetAngleDifference();
}

void cs::PhysicsSystem::UpdateComponents()
{
	for (auto pc : componentToCreate)
		CreateBody(pc);

	componentToCreate.clear();

	for (auto pc : componentToDestroy)
		DestroyBody(pc);

	componentToDestroy.clear();
}

cs::PhysicsSystem::PhysicsSystem() :
	world(new b2World({0.f, -9.81f})),
	listener(new PhysicsListener),
	velocityIterations(6),
	positionIterations(2),
	frequency(1.f/50.f),
	frequencyCounter(0.f)
{
	world->SetContactListener(listener);
}

cs::PhysicsSystem::~PhysicsSystem()
{
	delete world;
	delete listener;
}

void cs::PhysicsSystem::QueueComponentCreate(PhysicsComponent* pc)
{
	componentToCreate.push_back(pc);
}

void cs::PhysicsSystem::QueueBodyDestroy(PhysicsComponent* pc)
{
	componentToDestroy.push_back(pc);
}

void cs::PhysicsSystem::CreateBody(PhysicsComponent* pc)
{


	auto& _tc(pc->gameObject->GetComponentConst<TransformComponent>());
	Vector2 _position(_tc.position);
	float _angle(_tc.rotation.x);
	pc->definition.angle = _angle;
	pc->definition.position = { _position.x, _position.y };
	pc->delta.Teleport(_position, _angle);

	DestroyBody(pc);
	pc->body = world->CreateBody(&pc->definition);
	pc->UpdateShape();
	pc->UpdateFixtures();

	return;
}

void cs::PhysicsSystem::DestroyBody(PhysicsComponent* pc)
{
	// TODO: Possibly unneeded if statement
	if (pc->body)
	{
		world->DestroyBody(pc->body);
		pc->body = nullptr;
	}
}
