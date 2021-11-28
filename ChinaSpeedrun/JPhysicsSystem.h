#pragma once
#include "Mathf.h"

namespace cs
{
	struct JCollision;
	class JPhysicsComponent;
	class JCollisionSolver;

	class JPhysicsSystem
	{
	public:
		static constexpr float margin = 0.004f;
		static Vector3 gravity;
		static std::vector<JCollisionSolver> solvers;
		static std::vector<JCollision> collisions;
		static void CalculateMovement(JPhysicsComponent* pc);
		static void DetectCollision(JPhysicsComponent* pc1, JPhysicsComponent* pc2);
		static void SolveCollisions();
	};
}

