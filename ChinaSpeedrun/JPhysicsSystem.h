#pragma once
#include "Mathf.h"

namespace cs
{
	struct CollisionPoints;
	class JPhysicsComponent;
	class CollisionSolver;

	class JPhysicsSystem
	{
	public:
		static constexpr float margin = 0.004f;
		static Vector3 gravity;
		static std::vector<CollisionSolver> solvers;
		static std::vector<CollisionPoints> collisions;
		static void CalculateMovement(JPhysicsComponent* pc);
		static void DetectCollision(JPhysicsComponent* pc1, JPhysicsComponent* pc2);
		static void SolveCollisions();
	};
}

