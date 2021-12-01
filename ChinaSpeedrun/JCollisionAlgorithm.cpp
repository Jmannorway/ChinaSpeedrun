#include "JCollisionAlgorithm.h"

#include "JCollisionShape.h"
#include "Transform.h"
#include "Debug.h"

cs::JCollisionPoints cs::algo::FindSpherePlaneCollisionPoints(const JCollisionSphere* cs, const TransformComponent* cstc,
	const JCollisionPlane* cp, const TransformComponent* cptc)
{
	return JCollisionPoints();
}

cs::JCollisionPoints cs::algo::FindSphereTriangleCollisionPoints(const JCollisionSphere* cs,
	const TransformComponent* cstc, const JCollisionTriangle* ct, const TransformComponent* cttc)
{
	JCollisionPoints _cp;

	if (DistanceFromPointToPlane(cstc->position, ct->GetPoint(0) + cttc->position, ct->GetNormal()) <= cs->radius)
	{
		Vector3 _pointOnPlane = ProjectPointOntoPlane(cstc->position, ct->GetPoint(0), ct->GetNormal());
		float _shortestDistanceToLine = cs->radius + 1.f;

		for (unsigned i = 0; i < 3; i++)
		{
			Vector3 _pointOnLine = ClampPointOnLineToEnds(
				ProjectPointOnPlaneOntoLine(
					_pointOnPlane,
					ct->GetPoint(i) + cttc->position,
					ct->GetLineTangent(i)),
				ct->GetPoint(i) + cttc->position,
				ct->GetPoint((i + 1) % 3));

			_shortestDistanceToLine = glm::min(
				_shortestDistanceToLine,
				distance(cstc->position, _pointOnLine));
		}

		Debug::LogInfo(_shortestDistanceToLine);
		if (_shortestDistanceToLine <= cs->radius)
			Debug::LogInfo("Intersecting triangle lines");
		else
			Debug::LogInfo("Not intersecting triangle lines");
	}
	else
	{
		Debug::LogInfo("Not intersecting triangle lines");
	}
	return _cp;
}

cs::JCollisionPoints cs::algo::FindPlanePlaneCollisionPoints(
	const JCollisionPlane* cp1, const TransformComponent* cptc1, const JCollisionPlane* cp2, const TransformComponent* cptc2)
{
	return JCollisionPoints();
}

cs::JCollisionPoints cs::algo::FindPlaneTriangleCollisionPoints(
	const JCollisionPlane* cp, const TransformComponent* cptc, const JCollisionTriangle* ct, const TransformComponent* cttc)
{
	return JCollisionPoints();
}

cs::JCollisionPoints cs::algo::FindTriangleTriangleCollisionPoints(
	const JCollisionTriangle* ct1, const TransformComponent* cttc1, const JCollisionTriangle* ct2, const TransformComponent* cttc2)
{
	return JCollisionPoints();
}

cs::JCollisionPoints cs::algo::FindSphereSphereCollisionPoints(
	const JCollisionSphere* cs1, const TransformComponent* cstc1, const JCollisionSphere* cs2, const TransformComponent* cstc2)
{
	JCollisionPoints _points;
	const float _distance(distance(cstc1->position, cstc2->position));
	_points.normal = normalize(cstc2->position - cstc1->position);
	_points.a = cstc1->position + _points.normal * cs1->radius;
	_points.b = cstc2->position - _points.normal * cs2->radius;
	_points.depth = -_distance + cs1->radius + cs2->radius;
	_points.hasCollision = _distance <= cs1->radius + cs2->radius;
	Debug::LogInfo(_distance);
	return _points;
}
