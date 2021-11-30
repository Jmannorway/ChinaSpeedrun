#pragma once

#include "JCollisionPoints.h"

namespace cs
{
	class JPhysicsComponent;
	class TransformComponent;

	struct JCollision
	{
		JPhysicsComponent &pc1, &pc2;
		TransformComponent &tc1, &tc2;
		JCollisionPoints points;
		JCollision(JPhysicsComponent& pc1, TransformComponent& tc1, JPhysicsComponent& pc2, TransformComponent& tc2, JCollisionPoints points);
	};
}
