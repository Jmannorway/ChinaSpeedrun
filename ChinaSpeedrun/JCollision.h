#pragma once

#include "JCollisionPoints.h"

namespace cs
{
	class JPhysicsComponent;
	class TransformComponent;

	struct JCollision
	{
		TransformComponent* tc1, * tc2;
		JPhysicsComponent* pc1, * pc2;
		JCollisionPoints points;
		JCollision();
	};
}
