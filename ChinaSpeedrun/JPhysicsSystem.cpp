#include "JPhysicsSystem.h"
#include "JPhysicsComponent.h"
#include "GameObject.h"
#include "JCollisionShape.h"
#include "Transform.h"
#include "Time.h"
#include "JCollision.h"

Vector3 cs::JPhysicsSystem::gravity = Vector3(0.f, -9.8f, 0.f);
std::vector<cs::JCollision> cs::JPhysicsSystem::collisions;

void cs::JPhysicsSystem::CalculateMovement(JPhysicsComponent& pc)
{
	pc.velocity += pc.force;
	pc.force = Vector3(0.f);
	pc.velocity += (gravity * pc.gravityScale) * Time::deltaTime;
	auto& _tc = pc.gameObject->GetComponent<TransformComponent>();
	_tc.position += pc.velocity * Time::deltaTime;
}

void cs::JPhysicsSystem::DetectCollision(
	JPhysicsComponent& pc1, TransformComponent& tc1, JPhysicsComponent& pc2, TransformComponent& tc2)
{
	// TODO: Don't check collision against self
	if (&pc1 != &pc2)
	{
		JCollisionPoints _cp = pc1.shape->TestCollision(&tc1, pc2.shape, &tc2);

		if (_cp.hasCollision)
		{
			collisions.emplace_back(pc1, tc1, pc2, tc2, _cp);
		}
	}
}

void cs::JPhysicsSystem::SolveCollisions()
{
	// reflection solver
	for (auto& c : collisions)
	{
		c.at.position += c.points.normal * c.points.depth * glm::sign(c.a.gravityScale);
		c.bt.position += c.points.normal * c.points.depth * glm::sign(c.b.gravityScale);

		c.a.velocity -= Mathf::Project(c.points.normal, c.a.velocity);
		c.b.velocity -= Mathf::Project(c.points.normal, c.b.velocity);

		Debug::LogInfo("Calculations done!");
	}

	collisions.clear();
}

bool cs::JPhysicsSystem::ValidateComponent(JPhysicsComponent& pc)
{
	return (pc.shape != nullptr && pc.gameObject->HasComponent<TransformComponent>());
}