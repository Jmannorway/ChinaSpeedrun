#pragma once

#include "JCollisionAlgorithm.h"

namespace cs
{
	class RigidBodyComponent;
	class TransformComponent;

	struct Collision
	{
		TransformComponent* tc1, * tc2;
		RigidBodyComponent* rbc1, * rbc2;
		CollisionPoints points;
	};
}
