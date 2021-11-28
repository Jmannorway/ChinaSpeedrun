#pragma once

#include "Mathf.h"
#include "JCollisionPoints.h"

namespace cs
{
	class JCollisionSphere;
	class JCollisionShape;
	class JCollisionPlane;
	class JCollisionTriangle;
	class TransformComponent;

	namespace algo
	{
		JCollisionPoints FindSphereSphereCollisionPoints(
			const JCollisionSphere* cs1, const TransformComponent* cstc1, const JCollisionSphere* cs2, const TransformComponent* cstc2);
		JCollisionPoints FindSpherePlaneCollisionPoints(
			const JCollisionSphere* cs, const TransformComponent* cstc, const JCollisionPlane* cp, const TransformComponent* cptc);
		JCollisionPoints FindSphereTriangleCollisionPoints(
			const JCollisionSphere* cs, const TransformComponent* cstc, const JCollisionTriangle* ct, const TransformComponent* cttc);
		JCollisionPoints FindPlanePlaneCollisionPoints(
			const JCollisionPlane* cp1, const TransformComponent* cptc1, const JCollisionPlane* cp2, const TransformComponent* cptc2);
		JCollisionPoints FindPlaneTriangleCollisionPoints(
			const JCollisionPlane* cp, const TransformComponent* cptc, const JCollisionTriangle* ct, const TransformComponent* cttc);
		JCollisionPoints FindTriangleTriangleCollisionPoints(
			const JCollisionTriangle* ct1, const TransformComponent* cttc1, const JCollisionTriangle* ct2, const TransformComponent* cttc2);
	}
}
