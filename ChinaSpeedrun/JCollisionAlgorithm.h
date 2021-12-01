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
		inline float DistanceFromPointToPlaneSigned(Vector3 point, Vector3 pointOnPlane, Vector3 planeNormal)
		{
			return dot(point - pointOnPlane, planeNormal);
		}
		inline float DistanceFromPointToPlane(Vector3 point, Vector3 arbitraryPointOnPlane, Vector3 planeNormal)
		{
			return abs(DistanceFromPointToPlaneSigned(point, arbitraryPointOnPlane, planeNormal));
		}
		inline Vector3 ProjectPointOntoPlane(Vector3 point, Vector3 arbitraryPointOnPlane, Vector3 planeNormal)
		{
			return point - planeNormal * DistanceFromPointToPlaneSigned(point, arbitraryPointOnPlane, planeNormal);
		}
		inline float DistanceFromPointOnPlaneToLineSigned(Vector3 pointOnPlane, Vector3 pointOnLine, Vector3 lineTangent)
		{
			return dot(pointOnPlane - pointOnLine, lineTangent);
		}
		inline float DistanceFromPointOnPlaneToLine(Vector3 pointOnPlane, Vector3 pointOnLine, Vector3 lineTangent)
		{
			return abs(DistanceFromPointOnPlaneToLineSigned(pointOnPlane, pointOnLine, lineTangent));
		}
		inline Vector3 ProjectPointOnPlaneOntoLine(Vector3 pointOnPlane, Vector3 linePos, Vector3 lineTangent)
		{
			return pointOnPlane - lineTangent * DistanceFromPointOnPlaneToLineSigned(pointOnPlane, linePos, lineTangent);
		}
		inline Vector3 ClampPointOnLineToEnds(Vector3 pointOnLine, Vector3 linePos1, Vector3 linePos2)
		{
			return Vector3(
				glm::clamp(pointOnLine.x, glm::min(linePos1.x, linePos2.x), glm::max(linePos1.x, linePos2.x)),
				glm::clamp(pointOnLine.y, glm::min(linePos1.y, linePos2.y), glm::max(linePos1.y, linePos2.y)),
				glm::clamp(pointOnLine.z, glm::min(linePos1.z, linePos2.z), glm::max(linePos1.z, linePos2.z)));
		}
		inline float CircleEaseOut(float val)
		{
			return sqrt(1.f - pow(val - 1.f, 2.f));
		}

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
