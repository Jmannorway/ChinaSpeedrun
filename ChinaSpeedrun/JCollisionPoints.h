#pragma once
#include "Mathf.h"

namespace cs
{
	struct JCollisionPoints
	{
		Vector3 a, b, normal;
		float depth;
		bool hasCollision;
		JCollisionPoints();
	};
}
