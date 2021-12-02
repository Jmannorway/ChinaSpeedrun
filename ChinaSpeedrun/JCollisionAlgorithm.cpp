#include "JCollisionAlgorithm.h"

#include "JCollisionShape.h"
#include "Transform.h"
#include "Debug.h"

cs::JCollisionPoints cs::algo::FindSphereSphereCollisionPoints(
	const JCollisionSphere* cs1, const TransformComponent* cstc1, const JCollisionSphere* cs2, const TransformComponent* cstc2)
{
	JCollisionPoints _points;
	const float _distance(distance(cstc1->position, cstc2->position));
	_points.normal = (cstc2->position - cstc1->position) / _distance;
	_points.a = cstc1->position + _points.normal * cs1->radius;
	_points.b = cstc2->position - _points.normal * cs2->radius;
	_points.depth = -_distance + cs1->radius + cs2->radius;
	_points.hasCollision = _distance <= cs1->radius + cs2->radius;
	Debug::LogInfo(_distance);
	return _points;
}


cs::JCollisionPoints cs::algo::FindSpherePlaneCollisionPoints(const JCollisionSphere* cs, const TransformComponent* cstc,
	const JCollisionPlane* cp, const TransformComponent* cptc)
{
	JCollisionPoints _points;

	float _distanceToPlaneSigned = DistanceFromPointToPlaneSigned(cstc->position, cptc->position, cp->GetNormal());

	if (abs(_distanceToPlaneSigned) <= cs->radius)
	{
		_points.hasCollision = true;

		float _side = _distanceToPlaneSigned == 0.f ? 1.f : glm::sign(_distanceToPlaneSigned);
		_points.a = cstc->position - cp->GetNormal() * _side * cs->radius;
		_points.b = ProjectPointOntoPlane(cstc->position, cptc->position, cp->GetNormal());
		_points.normal = cp->GetNormal();
		_points.depth = distance(_points.a, _points.b);
	}

	Debug::LogInfo("Distance: ", abs(_distanceToPlaneSigned) - cs->radius);

	return _points;
}

cs::JCollisionPoints cs::algo::FindSphereTriangleCollisionPoints(const JCollisionSphere* cs,
	const TransformComponent* cstc, const JCollisionTriangle* ct, const TransformComponent* cttc)
{
	JCollisionPoints _cp;

	const Vector3 _arbitraryPointOnPlane = ct->GetPoint(0) + cttc->position;
	const Vector3 _triangleNormal = ct->GetNormal();
	float _distanceToPlane = DistanceFromPointToPlane(
		cstc->position, _arbitraryPointOnPlane, _triangleNormal);

	if (_distanceToPlane <= cs->radius)
	{
		const Vector3 _pointOnPlane = ProjectPointOntoPlane(cstc->position, _arbitraryPointOnPlane, _triangleNormal);
		float _shortestDistanceToLine = cs->radius + 1.f;

		for (unsigned i = 0; i < 3; i++)
		{
			Vector3 _pointOnLine = ClampPointOnLineToEnds(
				ProjectPointOnPlaneOntoLine(
					_pointOnPlane,
					ct->GetPoint(i) + cttc->position,
					ct->GetLineTangent(i)),
				ct->GetPoint(i) + cttc->position,
				ct->GetPoint((i + 1) % 3) + cttc->position);

			_shortestDistanceToLine = glm::min(
				_shortestDistanceToLine,
				distance(_pointOnPlane, _pointOnLine));
		}

		float _circleRadius = CircleEaseOut(1.f - (_distanceToPlane / cs->radius)) * cs->radius;
		bool _insideTriangle = IsPointOnPlaneInsideTriangle(
			cstc->position, cttc->position + ct->GetPoint(0), cttc->position + ct->GetPoint(1), cttc->position + ct->GetPoint(2));

		// Is colliding
		if (_shortestDistanceToLine <= _circleRadius || _insideTriangle)
		{
			_cp.hasCollision = true;

			// direction the objects need to be moved out of each other
			float _side = glm::sign(dot(cstc->position - _arbitraryPointOnPlane, _triangleNormal));
			_side = _side + (_side == 0.f) * 1.f;
			_cp.normal = _triangleNormal * _side;

			Debug::LogInfo(_shortestDistanceToLine, ", ", _circleRadius, ", Side: ", _side);

			// the first point of contact on the sphere
			_cp.a = cstc->position - cs->radius * _triangleNormal;

			// the first point of contact on the triangle
			_cp.b = cstc->position - DistanceFromPointToPlane(
				cstc->position, _arbitraryPointOnPlane, _triangleNormal) * _triangleNormal;

			// The distance from a to b
			_cp.depth = distance(_cp.a, _cp.b);
		}
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