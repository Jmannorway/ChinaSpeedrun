#pragma once

#include "JCollisionPoints.h"

namespace cs
{
	class JPhysicsComponent;
	class TransformComponent;

	struct JCollision
	{
		JPhysicsComponent &a, &b;
		TransformComponent &at, &bt;
		JCollisionPoints points;
		JCollision(JPhysicsComponent& a, TransformComponent& at, JPhysicsComponent& b, TransformComponent& bt, JCollisionPoints points);
	};
}
