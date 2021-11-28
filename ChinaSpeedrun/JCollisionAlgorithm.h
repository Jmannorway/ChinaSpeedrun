#pragma once

#include "Mathf.h"
#include "JCollisionPoints.h"

namespace cs
{
	class CollisionSphere;
	class CollisionShape;
	class CollisionPlane;
	class CollisionTriangle;
	class TransformComponent;

	namespace algo
	{
		CollisionPoints FindSphereSphereCollisionPoints(
			const CollisionSphere* cs1, const TransformComponent* cstc1, const CollisionSphere* cs2, const TransformComponent* cstc2);
		CollisionPoints FindSpherePlaneCollisionPoints(
			const CollisionSphere* cs, const TransformComponent* cstc, const CollisionPlane* cp, const TransformComponent* cptc);
		CollisionPoints FindSphereTriangleCollisionPoints(
			const CollisionSphere* cs, const TransformComponent* cstc, const CollisionTriangle* ct, const TransformComponent* cttc);
		CollisionPoints FindPlanePlaneCollisionPoints(
			const CollisionPlane* cp1, const TransformComponent* cptc1, const CollisionPlane* cp2, const TransformComponent* cptc2);
		CollisionPoints FindPlaneTriangleCollisionPoints(
			const CollisionPlane* cp, const TransformComponent* cptc, const CollisionTriangle* ct, const TransformComponent* cttc);
		CollisionPoints FindTriangleTriangleCollisionPoints(
			const CollisionTriangle* ct1, const TransformComponent* cttc1, const CollisionTriangle* ct2, const TransformComponent* cttc2);
	}
}
