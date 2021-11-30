#pragma once
#include "Mathf.h"

namespace cs
{
	class TransformComponent;
	class JPhysicsComponent;
	struct JCollision;

	class JPhysicsSystem
	{
	public:
		static constexpr float margin = 0.004f;
		static Vector3 gravity;

		// Regular update functions
		static void CalculateMovement(JPhysicsComponent& pc);
		static void DetectCollision(
			JPhysicsComponent& pc1, TransformComponent& tc1, JPhysicsComponent& pc2, TransformComponent& tc2);
		static void SolveCollisions();
	private:
		static bool ValidateComponent(JPhysicsComponent& pc);
		static std::vector<JCollision> collisions;
	};
}

