#pragma once

#include "Mathf.h"

namespace cs
{
	class CollisionSphere;
	class CollisionShape;
	class CollisionPlane;
	class TransformComponent;

	struct CollisionPoints
	{
		Vector3 a, b, normal;
		float depth;
		bool hasCollision;
		CollisionPoints();
	};

	namespace algo
	{
		CollisionPoints FindSpherePlaneCollisionPoints(
			const CollisionSphere* cs, const TransformComponent* cstc, const CollisionPlane* cp, const TransformComponent* cptc);
		CollisionPoints FindPlanePlaneCollisionPoints(
			const CollisionPlane* cp1, const TransformComponent* cptc1, const CollisionPlane* cp2, const TransformComponent* cptc2);
		CollisionPoints FindSphereSphereCollisionPoints(
			const CollisionSphere* cs1, const TransformComponent* cstc1, const CollisionSphere* cs2, const TransformComponent* cstc2);
	}
}
